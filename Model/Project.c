//
// Created by maxime on 24/03/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "Project.h"
#include "VertexList.h"
#include "EdgeList.h"
#include "Card.h"
#include "Link.h"

static int copyFile(char* dest, char* source);

/**
 * Allocate a unlock project in the dynamic space memory
 * @return the address of the allocated structure
 */
Project* allocProject(void){
    Project* newP = (Project*) malloc(sizeof(Project));
    return newP;
}

/**
 * Free a project
 * @param p the project to free
 */
void freeProject(Project* p){
    free(p);
}

/**
 * Initialize an empty project and create its directory tree at the specified absolute path
 * Set the id arrays to arrays of 0
 * @param p the project to initialize
 * @param path the absolute path where the project's directory has to be created
 * @param name the name of the project
 * @return 0 if it's a success, -1 if not
 */
int initProject(Project* p, char* path, char* name){

    int i;
    char mainDirectory[MAXPATH+1+MAXNAME+1];
    char projectDirectory[MAXPATH+1+MAXNAME+1+7+1];
    char cardsDirectory[MAXPATH+1+MAXNAME+1+5+1];

    if (p != NULL ){

        strncpy(p->path, path, MAXPATH);
        p->path[MAXPATH] = '\0';
        strncpy(p->name, name, MAXNAME);
        p->name[MAXNAME] = '\0';

        p->root = NULL;

        initEmptyVertexList(&p->cardList);
        p->nbCards = 0;
        for(i=0;i<MAXCARD;i++){
            p->idCards[i] = 0;
        }

        initEmptyEdgeList(&p->linkList);
        p->nbLinks = 0;
        for(i=0;i<MAXLINK;i++){
            p->idLinks[i] = 0;
        }

        p->backImage = 0;
        p->topImage = 0;
        p->bottomImage = 0;

        // creation of the directory tree

        //first directory : [projectPath]/[projectName]

        strncpy(mainDirectory, p->path, MAXPATH);
        strcat(mainDirectory,"/");
        strncat(mainDirectory, p->name, MAXNAME);

        if ( mkdir(mainDirectory, 0755 ) != 0 ) {
            fprintf( stderr, "error : creation of the directory %s is impossible\n", mainDirectory);
            fprintf(stderr,"%d\n", errno);
            return -1;
        }

        //second directory : [projectPath]/[projectName]/Project

        strncpy(projectDirectory, mainDirectory, MAXPATH+1+MAXNAME+1);
        strcat(projectDirectory, "/Project");

        if ( mkdir(projectDirectory, 0755 ) != 0 ) {
            fprintf( stderr, "error : creation of the directory %s is impossible\n", projectDirectory);
            fprintf(stderr,"%d\n", errno);
            return -1;
        }

        //third directory : [projectPath]/[projectName]/Cards

        strncpy(cardsDirectory, mainDirectory, MAXPATH+1+MAXNAME+1);
        strcat(cardsDirectory, "/Cards");

        if ( mkdir(cardsDirectory, 0755 ) != 0 ) {
            fprintf( stderr, "error : creation of the directory %s is impossible\n", cardsDirectory);
            fprintf(stderr,"%d\n", errno);
            return -1;
        }

        return 0;

    } else {
        fprintf(stderr, "error : project bad allocation\n");
        return -1;
    }
}

/**
 * Initialize an empty project without create its directory tree (already exists)
 * Set the id arrays to arrays of 0
 * @param p the project to initialized
 * @param path the absolute path of the project
 * @param name the name of the project
 * @return 0 if it's a success, -1 if not
 */
int initProjectForLoad(Project* p, char* path, char* name){

    int i;

    if (p != NULL ){

        strncpy(p->path, path, MAXPATH);
        p->path[MAXPATH] = '\0';
        strncpy(p->name, name, MAXNAME);
        p->name[MAXNAME] = '\0';

        p->root = NULL;

        initEmptyVertexList(&p->cardList);
        p->nbCards = 0;
        for(i=0;i<MAXCARD;i++){
            p->idCards[i] = 0;
        }

        initEmptyEdgeList(&p->linkList);
        p->nbLinks = 0;
        for(i=0;i<MAXLINK;i++){
            p->idLinks[i] = 0;
        }

        p->backImage = 0;
        p->topImage = 0;
        p->bottomImage = 0;

        return 0;

    } else {
        fprintf(stderr, "error : project bad allocation\n");
        return -1;
    }
}


/**
 * Free every structures (cards, links, vertexList,...) contained by a project
 * Delete directory tree of the project
 * @param p the project to delete
 * @return 0 if it's a success, -1 if not
 */
int deleteProject(Project* p){

    char mainDirectory[MAXPATH+1+MAXNAME+1];
    char projectDirectory[MAXPATH+1+MAXNAME+1+7+1];
    char cardsDirectory[MAXPATH+1+MAXNAME+1+5+1];

    char backupFile[MAXPATH+1+MAXNAME+1+MAXNAME+4+1];
    char printFile[MAXPATH+1+MAXNAME+1+MAXNAME+4+1];

    char backImage[MAXPATH+1+MAXNAME+1+7+1+13+1];
    char topImage[MAXPATH+1+MAXNAME+1+7+1+12+1];
    char bottomImage[MAXPATH+1+MAXNAME+1+7+1+15+1];

    if (p != NULL ){

        setOnFirstEdge(&p->linkList);
        while(!isOutOfListEdge(&p->linkList)){
            deleteLink(p,p->linkList.current->link);
            setOnNextEdge(&p->linkList);
        }

        setOnFirstVertex(&p->cardList);
        while(!isOutOfListVertex(&p->cardList)){
            deleteCard(p,p->cardList.current->card);
            setOnNextVertex(&p->cardList);
        }

        //first directory : [projectPath]/[projectName]

        strncpy(mainDirectory, p->path, MAXPATH);
        strcat(mainDirectory,"/");
        strncat(mainDirectory, p->name, MAXNAME);

        //second directory : [projectPath]/[projectName]/Project

        strncpy(projectDirectory, mainDirectory, MAXPATH+1+MAXNAME+1);
        strcat(projectDirectory, "/Project");

        //third directory : [projectPath]/[projectName]/Cards

        strncpy(cardsDirectory, mainDirectory, MAXPATH+1+MAXNAME+1);
        strcat(cardsDirectory, "/Cards");

        //project backup file

        strncpy(backupFile, mainDirectory, MAXPATH+1+MAXNAME+1);
        strcat(backupFile,"/");
        strncat(backupFile, p->name, MAXNAME);
        strcat(backupFile,".txt");

        //project printable file

        strncpy(printFile, mainDirectory, MAXPATH+1+MAXNAME+1);
        strcat(printFile,"/");
        strncat(printFile, p->name, MAXNAME);
        strcat(printFile,".pdf");

        //project images

        strncpy(backImage, projectDirectory, MAXPATH+1+MAXNAME+1+7+1);
        strcat(backImage, "/BackImage.jpg");

        strncpy(topImage, projectDirectory, MAXPATH+1+MAXNAME+1+7+1);
        strcat(topImage, "/TopImage.jpg");

        strncpy(bottomImage, projectDirectory, MAXPATH+1+MAXNAME+1+7+1);
        strcat(bottomImage, "/BottomImage.jpg");

        //remove project images files

        if(remove(backImage) != 0){
            fprintf( stderr, "error : deletion of the file %s is impossible\n", backImage);
            fprintf(stderr,"%d\n", errno);
        }

        if(remove(topImage) != 0){
            fprintf( stderr, "error : deletion of the file %s is impossible\n", topImage);
            fprintf(stderr,"%d\n", errno);
        }

        if(remove(bottomImage) != 0){
            fprintf( stderr, "error : deletion of the file %s is impossible\n", bottomImage);
            fprintf(stderr,"%d\n", errno);
        }

        // remove project backup and print files

        if(remove(backupFile) != 0){
            fprintf( stderr, "error : deletion of the file %s is impossible\n", backupFile);
            fprintf(stderr,"%d\n", errno);
        }

        if(remove(printFile) != 0){
            fprintf( stderr, "error : deletion of the file %s is impossible\n", printFile);
            fprintf(stderr,"%d\n", errno);
        }

        // remove directories

        if(rmdir(cardsDirectory) != 0){
            fprintf( stderr, "error : deletion of the directory %s is impossible\n", cardsDirectory);
            fprintf(stderr,"%d\n", errno);
            return -1;
        }

        if(rmdir(projectDirectory) != 0){
            fprintf( stderr, "error : deletion of the directory %s is impossible\n", projectDirectory);
            fprintf(stderr,"%d\n", errno);
            return -1;
        }

        if(rmdir(mainDirectory) != 0){
            fprintf( stderr, "error : deletion of the directory %s is impossible\n", mainDirectory);
            fprintf(stderr,"%d\n", errno);
            return -1;
        }

        return 0;

    } else {
        fprintf(stderr, "error : project bad allocation\n");
        return -1;
    }
}

/**
 * Set the root card (the unique face up card at the beginning of the game) of a project
 * @param p the project whose the root has to be set
 * @param root the new root card for the project
 */
void setRoot(Project* p, Card* root){
    if (p != NULL && root != NULL ){
        p->root = root;
    } else {
        fprintf(stderr, "error : project or root card bad allocation\n");
    }
}

/**
 * Automatically assign a free id to a card and set it as occupied in the idCards array of a project
 * @param p the project
 * @param c the card
 * @return 0 if it's a success, -1 if not
 */
int assignIdCard(Project* p, Card* c){

    int i;

    if (p != NULL && c != NULL){

        for(i=0;i<MAXCARD;i++){
            if(p->idCards[i] == 0){
                c->id = i;
                p->idCards[i] = 1;
                return 0;
            }
        }

        fprintf(stderr, "error : impossible to assign an id to the card\n");
        return -1;

    } else {
        fprintf(stderr, "error : project or card bad allocation\n");
        return -1;
    }
}

/**
 * Assign a given id (verify it is free) to a card and set it as occupied in the idCards array of a project
 * @param p the project
 * @param c the card
 * @param id the id to assign
 * @return 0 if it's a success, -1 if not
 */
int assignGivenIdCard(Project* p, Card* c, int id){

    if (p != NULL && c != NULL){

        if(id < 0 && id >= MAXCARD){
            fprintf(stderr, "error :  the given id hasn't a correct value\n");
            return -1;
        }

        if(p->idCards[id] == 0) {
            c->id = id;
            p->idCards[id] = 1;
            return 0;
        } else {
            fprintf(stderr, "error : impossible to assign the given id to the card (already used)\n");
            return -1;
        }

    } else {
        fprintf(stderr, "error : project or card bad allocation\n");
        return -1;
    }
}

/**
 * Automatically unassign an id from a card => set it as unoccupied in the idCards array of a project
 * @param p the project
 * @param c the card
 */
void unassignIdCard(Project* p, Card* c){

    if (p != NULL && c != NULL){

        p->idCards[c->id] = 0;

    } else {
        fprintf(stderr, "error : project or card bad allocation\n");
    }
}

/**
 * Add an empty card to a project
 * Create its directory into the directory tree of the project
 * @param p the project to which a card must be added
 * @return a pointer towards the created card
 */
Card* addEmptyCard(Project* p){

    char pathCardDirectory[MAXPATH+1+MAXNAME+1+5+1+4+TAILLEMAXID+1];

    char idString[TAILLEMAXID];

    if (p != NULL){

        if(p->nbCards >= MAXCARD){
            fprintf(stderr, "error : maximum number of cards in a project reached\n");
            return NULL;
        }

        Card* c = allocCard();

        if(c == NULL){
            fprintf(stderr, "error : card bad allocation\n");
            return NULL;
        }

        initEmptyCard(c);

        // adding the card to the project

        insertVertexLast(&p->cardList, c);
        p->nbCards ++;
        if(assignIdCard(p,c) != 0){
            freeCard(c);
            return NULL;
        }

        // construction of the path

        strncpy(pathCardDirectory, p->path, MAXPATH);
        strcat(pathCardDirectory, "/");
        strncat(pathCardDirectory, p->name, MAXNAME);
        strcat(pathCardDirectory, "/Cards/Card");
        sprintf(idString, "%d", c->id);
        strncat(pathCardDirectory, idString, TAILLEMAXID);

        // creation of the directory

        if ( mkdir(pathCardDirectory, 0755 ) != 0 ) {
            fprintf( stderr, "error : creation of the directory %s is impossible\n", pathCardDirectory);
            fprintf(stderr,"%d\n", errno);
            if(errno==EEXIST) {
                //if the card's file already existed, it is emptied to be used as new
                char pathCardImage[MAXPATH+1+MAXNAME+1+5+1+4+TAILLEMAXID+1+9+1];
                strncpy(pathCardImage, pathCardDirectory, MAXPATH+1+MAXNAME+1+5+1+4+TAILLEMAXID+1);
                strcat(pathCardImage, "/Image.jpg");

                if(remove(pathCardImage) != 0){
                    fprintf( stderr, "error : deletion of the file %s is impossible\n", pathCardImage);
                    fprintf(stderr,"%d\n", errno);
                }
            } else {
                //if the error is something else, delete the card in the project's card list
                deleteVertex(&p->cardList, c);
                p->nbCards --;
                freeCard(c);
                return NULL;
            }
        }

        return c;

    } else {
        fprintf(stderr, "error : project bad allocation\n");
        return NULL;
    }
}

/**
 * Create and add a card to a project when it is loaded from a .txt file
 * Don't create a directory for the card (already existed)
 * The id of the card created is the given id
 * @param p the project
 * @param id the id of the new card
 * @param type the type of the new card
 * @param number the number of the new card
 * @param fixedNumber the enable fixedNumber of the new card
 * @param image the enable image of the new card
 * @return a pointer towards the created card
 */
Card* addCardForLoad(Project* p, int id, cardType type, int number, int fixedNumber, int image){

    if (p != NULL){

        if(p->nbCards >= MAXCARD){
            fprintf(stderr, "error : maximum number of cards in a project reached\n");
            return NULL;
        }

        Card* c = allocCard();

        if(c == NULL){
            fprintf(stderr, "error : card bad allocation\n");
            return NULL;
        }

        initEmptyCard(c);

        // filling the card

        c->type = type;
        c->number = number;
        c->fixedNumber = fixedNumber;
        c->image = image;

        // adding the card to the project

        insertVertexLast(&p->cardList, c);
        p->nbCards ++;
        if(assignGivenIdCard(p,c,id) != 0){
            freeCard(c);
            return NULL;
        }

        return c;

    } else {
        fprintf(stderr, "error : project bad allocation\n");
        return NULL;
    }
}

/**
 * Set a front image for a card of the project
 * Copy an image to the directory /[projectPath]/Project/Cards/Card[c->id] and rename it Image.jpg
 * Set the enable image for the card to 1 if it's a success
 * @param p the project containing the card c
 * @param c the card
 * @param pathImage the absolute path of the image to copy
 * @return 0 if it's a success, -1 if not
 */
int setCardImage(Project* p, Card* c, char* pathImage){

    char pathCardImage[MAXPATH+1+MAXNAME+1+5+1+4+TAILLEMAXID+1+9+1];

    char idString[TAILLEMAXID];

    if (p != NULL && c != NULL){

        // construction of the new path

        strncpy(pathCardImage, p->path, MAXPATH);
        strcat(pathCardImage, "/");
        strncat(pathCardImage, p->name, MAXNAME);
        strcat(pathCardImage, "/Cards/Card");
        sprintf(idString, "%d", c->id);
        strncat(pathCardImage, idString, TAILLEMAXID);
        strcat(pathCardImage, "/Image.jpg");

        // copy of the file

        if(copyFile(pathCardImage, pathImage) != 0){
            fprintf(stderr, "error : failure in the copy of the file\n");
            return -1;
        }

        c->image = 1;

        return 0;

    } else {
        fprintf(stderr, "error : project or card bad allocation\n");
        return -1;
    }
}

/**
 * Remove a card from a project and free it
 * Must delete all links in which the card is included
 * Delete its directory in the directory tree
 * @param p the project to which a card must be removed
 * @param c the card to delete
 * @return 0 if it's a success, -1 if not
 */
int deleteCard(Project* p, Card* c){

    Link* currentLink;

    char pathCardDirectory[MAXPATH+1+MAXNAME+1+5+1+4+TAILLEMAXID+1];
    char pathCardImage[MAXPATH+1+MAXNAME+1+5+1+4+TAILLEMAXID+1+9+1];
    char idString[TAILLEMAXID];

    if (p != NULL && c != NULL){

        // deletion of all links in which the card c is included

        setOnFirstEdge(&p->linkList);

        while(!isOutOfListEdge(&p->linkList)){

            currentLink = p->linkList.current->link;

            if(currentLink->parent == c || currentLink->child == c){
                deleteLink(p,currentLink);
            }

            setOnNextEdge(&p->linkList);
        }

        // deletion of the image of the card (if it exists) and of the directory of the card

        strncpy(pathCardDirectory, p->path, MAXPATH);
        strcat(pathCardDirectory, "/");
        strncat(pathCardDirectory, p->name, MAXNAME);
        strcat(pathCardDirectory, "/Cards/Card");
        sprintf(idString, "%d", c->id);
        strncat(pathCardDirectory, idString, TAILLEMAXID);

        strncpy(pathCardImage, pathCardDirectory, MAXPATH+1+MAXNAME+1+5+1+4+TAILLEMAXID+1);
        strcat(pathCardImage, "/Image.jpg");

        if(remove(pathCardImage) != 0){
            fprintf( stderr, "error : deletion of the file %s is impossible\n", pathCardImage);
            fprintf(stderr,"%d\n", errno);
        }

        if(rmdir(pathCardDirectory) != 0){
            fprintf( stderr, "error : deletion of the directory %s is impossible\n", pathCardDirectory);
            fprintf(stderr,"%d\n", errno);
            return -1;
        }

        // deletion of the card from the card list of the project

        if(deleteVertex(&p->cardList,c) != 0){
            fprintf(stderr, "error : problem during the card %d deletion\n", c->id);
            return -1;
        }
        p->nbCards --;
        unassignIdCard(p,c);

        // freeing the card

        freeCard(c);
        //c = NULL;

        return 0;

    } else {
        fprintf(stderr, "error : project or card bad allocation\n");
        return -1;
    }
}

/**
 * Automatically assign a free id to a link and set it as occupied in the idLinks array of a project
 * @param p the project
 * @param l the link
 * @return 0 if it's a success, -1 if not
 */
int assignIdLink(Project* p, Link* l){
    int i;
    if (p != NULL && l != NULL){

        for(i=0;i<MAXLINK;i++){
            if(p->idLinks[i] == 0){
                l->id = i;
                p->idLinks[i] = 1;
                return 0;
            }
        }

        fprintf(stderr, "error : impossible to assign an id to the link\n");
        return -1;

    } else {
        fprintf(stderr, "error : project or link bad allocation\n");
        return -1;
    }
}

/**
 * Assign a given id (verify it is free) to a link and set it as occupied in the idLinks array of a project
 * @param p the project
 * @param l the link
 * @param id the id to assign
 * @return 0 if it's a success, -1 if not
 */
int assignGivenIdLink(Project* p, Link* l, int id){

    if (p != NULL && l != NULL){

        if(id < 0 && id >= MAXLINK){
            fprintf(stderr, "error :  the given id hasn't a correct value\n");
            return -1;
        }

        if(p->idLinks[id] == 0) {
            l->id = id;
            p->idLinks[id] = 1;
            return 0;
        } else {
            fprintf(stderr, "error : impossible to assign the given id to the link (already used)\n");
            return -1;
        }

    } else {
        fprintf(stderr, "error : project or card bad allocation\n");
        return -1;
    }
}

/**
 * Automatically unassign an id from a link => set it as unoccupied in the idLinks array of a project
 * @param p the project
 * @param l the link
 */
void unassignIdLink(Project* p, Link* l){

    if (p != NULL && l != NULL){

        p->idLinks[l->id] = 0;

    } else {
        fprintf(stderr, "error : project or link bad allocation\n");
    }
}

/**
 * Add a link between 2 cards of a project
 * @param p the project to which a card must be added
 * @param parent the parent card of the link
 * @param child the child card of the link
 * @param type the type of the link
 * @return a pointer towards the link created
 */
Link* addLink(Project* p, Card* parent, Card* child, linkType type){

    if (p != NULL && parent != NULL && child != NULL){

        if(p->nbLinks >= MAXLINK){
            fprintf(stderr, "error : maximum number of links in a project reached\n");
            return NULL;
        }

        Link* l = allocLink();

        if(l == NULL){
            fprintf(stderr, "error : link bad allocation\n");
            return NULL;
        }

        initEmptyLink(l);

        // filling the link

        setParent(l, parent);
        setChild(l, child);
        setLinkType(l, type);

        // adding the link to the link list of the cards parent and child

        insertEdgeLast(&parent->children, l);
        insertEdgeLast(&child->parents, l);

        // adding the link to the project

        insertEdgeLast(&p->linkList, l);
        p->nbLinks ++;
        if(assignIdLink(p,l) != 0){
            freeLink(l);
            return NULL;
        }

        return l;

    } else {
        fprintf(stderr, "error : project or parent card or child card bad allocation\n");
        return NULL;
    }
}

/**
 * Create and add a link between 2 cards of a project
 * The id of the link created is the given id
 * @param p the project
 * @param id the given id for the new link
 * @param type the type of the new link
 * @param parent the parent card of the new link
 * @param child the child card of the new link
 * @return a pointer towards the link created
 */
Link* addLinkForLoad(Project* p, int id, linkType type, Card* parent, Card* child){

    if (p != NULL && parent != NULL && child != NULL){

        if(p->nbLinks >= MAXLINK){
            fprintf(stderr, "error : maximum number of links in a project reached\n");
            return NULL;
        }

        Link* l = allocLink();

        if(l == NULL){
            fprintf(stderr, "error : link bad allocation\n");
            return NULL;
        }

        initEmptyLink(l);

        // filling the link

        setLinkType(l, type);
        setParent(l, parent);
        setChild(l, child);

        // adding the link to the link list of the cards parent and child

        insertEdgeLast(&parent->children, l);
        insertEdgeLast(&child->parents, l);

        // adding the link to the project

        insertEdgeLast(&p->linkList, l);
        p->nbLinks ++;
        if(assignGivenIdLink(p,l,id) != 0){
            freeLink(l);
            return NULL;
        }

        return l;

    } else {
        fprintf(stderr, "error : project or parent card or child card bad allocation\n");
        return NULL;
    }
}

/**
 * Remove a link from a project and free it
 * @param p the project to which a link must be removed
 * @param l the link to delete
 * @return 0 if it's a success, -1 if not
 */
int deleteLink(Project* p, Link* l){

    if (p != NULL && l != NULL){

        if(deleteEdge(&l->parent->children,l) != 0){
            fprintf(stderr, "error : problem during the link %d deletion\n", l->id);
            return -1;
        }
        if(deleteEdge(&l->child->parents,l) != 0){
            fprintf(stderr, "error : problem during the link %d deletion\n", l->id);
            return -1;
        }

        if(deleteEdge(&p->linkList,l) != 0){
            fprintf(stderr, "error : problem during the link %d deletion\n", l->id);
            return -1;
        }
        p->nbLinks --;
        unassignIdLink(p,l);

        freeLink(l);

        return 0;

    } else {
        fprintf(stderr, "error : project or link bad allocation\n");
        return -1;
    }
}

/**
 * Remove a link from a project and free it
 * @param p the project to which a link must be removed
 * @param parent the parent card of the link to delete
 * @param child the child card of the link to delete
 * @return 0 if it's a success, -1 if not
 */
int deleteLinkFromCards(Project* p, Card* parent, Card* child){

    Link* l;

    if (p != NULL && parent != NULL && child != NULL){

        setOnFirstEdge(&p->linkList);

        while(!isOutOfListEdge(&p->linkList)){

            l = p->linkList.current->link;

            if(l->parent == parent && l->child == child){
                return deleteLink(p, l);
            }

            setOnNextEdge(&p->linkList);
        }

        fprintf(stderr, "error : there is no link with this parent card and this child card\n");
        return -1;

    } else {
        fprintf(stderr, "error : project or parent card or child card bad allocation\n");
        return -1;
    }
}

/**
 * Copy the content of the source file into the dest file (and create the dest file if it doesn't exist)
 * @param dest the path of the destination file
 * @param source the path of the source file
 * @return 0 if it's a success, -1 if not
 */
static int copyFile(char* dest, char* source){

    FILE* sourceFile;
    FILE* destFile;
    unsigned char* buffer[1024];
    size_t nbBlocksRead;

    if((sourceFile = fopen(source,"rb")) == (FILE*) NULL){
        fprintf(stderr, "error : impossible to open the source file\n");
        perror("msg : ");
        return -1;
    }

    if((destFile = fopen(dest,"wb")) == (FILE*) NULL){
        fprintf(stderr, "error : impossible to open the destination file\n");
        perror("msg : ");
        fclose(sourceFile);
        return -1;
    }

    while((nbBlocksRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0){
        fwrite(buffer, 1, nbBlocksRead, destFile);
    }

    fclose(sourceFile);
    fclose(destFile);

    return 0;
}

/**
 * Set a back image for all cards of the project
 * Copy an image to the directory /[projectPath]/Project and rename it BackImage.jpg
 * Set the enable backImage to 1 if it's a success
 * @param p the project
 * @param path the absolute path of the image to copy
 * @return 0 if it's a success, -1 if not
 */
int setBackImage(Project* p, char* pathImage){

    char pathBackImage[MAXPATH+1+MAXNAME+1+7+1+13+1];

    if (p != NULL) {

        // construction of the new path

        strncpy(pathBackImage, p->path, MAXPATH);
        strcat(pathBackImage, "/");
        strncat(pathBackImage, p->name, MAXNAME);
        strcat(pathBackImage, "/Project");
        strcat(pathBackImage, "/BackImage.jpg");

        // copy of the file

        if(copyFile(pathBackImage, pathImage) != 0){
            fprintf(stderr, "error : failure in the copy of the file\n");
            return -1;
        }

        p->backImage = 1;

        return 0;

    } else {
        fprintf(stderr, "error : project bad allocation\n");
        return -1;
    }
}

/**
 * Set a top image for all cards of the project
 * Copy an image to the directory /[projectPath]/Project and rename it TopImage.jpg
 * Set the enable topImage to 1 if it's a success
 * @param p the project
 * @param path the absolute path of the image to copy
 * @return 0 if it's a success, -1 if not
 */
int setTopImage(Project* p, char* pathImage){

    char pathTopImage[MAXPATH+1+MAXNAME+1+7+1+12+1];

    if (p != NULL) {

        // construction of the new path

        strncpy(pathTopImage, p->path, MAXPATH);
        strcat(pathTopImage, "/");
        strncat(pathTopImage, p->name, MAXNAME);
        strcat(pathTopImage, "/Project");
        strcat(pathTopImage, "/TopImage.jpg");

        // copy of the file

        if(copyFile(pathTopImage, pathImage) != 0){
            fprintf(stderr, "error : failure in the copy of the file\n");
            return -1;
        }

        p->topImage = 1;

        return 0;

    } else {
        fprintf(stderr, "error : project bad allocation\n");
        return -1;
    }
}

/**
 * Set a bottom image for all cards of the project
 * Copy an image to the directory /[projectPath]/Project and rename it BottomImage.jpg
 * Set the enable bottomImage to 1 if it's a success
 * @param p the project
 * @param path the absolute path of the image to copy
 * @return 0 if it's a success, -1 if not
 */
int setBottomImage(Project* p, char* pathImage){

    char pathBottomImage[MAXPATH+1+MAXNAME+1+7+1+15+1];

    if (p != NULL) {

        // construction of the new path

        strncpy(pathBottomImage, p->path, MAXPATH);
        strcat(pathBottomImage, "/");
        strncat(pathBottomImage, p->name, MAXNAME);
        strcat(pathBottomImage, "/Project");
        strcat(pathBottomImage, "/BottomImage.jpg");

        // copy of the file

        if(copyFile(pathBottomImage, pathImage) != 0){
            fprintf(stderr, "error : failure in the copy of the file\n");
            return -1;
        }

        p->bottomImage = 1;

        return 0;

    } else {
        fprintf(stderr, "error : project bad allocation\n");
        return -1;
    }
}

/**
 * Save all the information about a project in a [projectName].txt in the /[projectPath] directory
 * The information stored has to respect a precise and specific template in order to be read when loading a project
 * If the .txt file doesn't exist, create it and if it already exists, suppress it and recreate it
 * @param p the project to save
 * @return 0 if it's a success, -1 if not
 */
int saveProject(Project* p){

    char pathBackupFile[MAXPATH+1+MAXNAME+1+MAXNAME+4+1];
    FILE* backupFile;

    if (p != NULL) {

        // construction of the backup file path

        strncpy(pathBackupFile, p->path, MAXPATH);
        strcat(pathBackupFile, "/");
        strncat(pathBackupFile, p->name, MAXNAME);
        strcat(pathBackupFile, "/");
        strncat(pathBackupFile, p->name, MAXNAME);
        strcat(pathBackupFile,".txt");

        // remove precedent file if it existed

        if(remove(pathBackupFile) != 0){
            fprintf( stderr, "error : deletion of the file %s is impossible\n", pathBackupFile);
            fprintf(stderr,"%d\n", errno);
        }

        // creation and completion of the new file

        if((backupFile = fopen(pathBackupFile,"w")) == (FILE*) NULL){
            fprintf(stderr, "error : impossible to open the backup file\n");
            perror("msg : ");
            return -1;
        }

        fprintf(backupFile,"backup file for a unlock project :\n");

        if(p->root != NULL){
            fprintf(backupFile,"%d;", p->root->id);
        } else {
            fprintf(backupFile,"-1;");
        }
        fprintf(backupFile,"%d;", p->backImage);
        fprintf(backupFile,"%d;", p->topImage);
        fprintf(backupFile,"%d\n", p->bottomImage);

        if(!isVertexListEmpty(&p->cardList)){

            setOnFirstVertex(&p->cardList);

            while(!isOutOfListVertex(&p->cardList)){

                if(p->cardList.current->card == NULL){
                    fprintf(stderr, "error : card bad allocation\n");
                    fclose(backupFile);
                    return -1;
                }

                fprintf(backupFile,".\n");
                fprintf(backupFile, "%d;", p->cardList.current->card->id);
                fprintf(backupFile, "%d;", p->cardList.current->card->type);
                fprintf(backupFile, "%d;", p->cardList.current->card->number);
                fprintf(backupFile, "%d;", p->cardList.current->card->fixedNumber);
                fprintf(backupFile, "%d\n", p->cardList.current->card->image);

                setOnNextVertex(&p->cardList);
            }
        }

        fprintf(backupFile,"!\n");

        if(!isEdgeListEmpty(&p->linkList)){

            setOnFirstEdge(&p->linkList);

            while(!isOutOfListEdge(&p->linkList)){

                if(p->linkList.current->link == NULL){
                    fprintf(stderr, "error : link bad allocation\n");
                    fclose(backupFile);
                    return -1;
                }

                fprintf(backupFile,".\n");
                fprintf(backupFile, "%d;", p->linkList.current->link->id);
                fprintf(backupFile, "%d;", p->linkList.current->link->type);
                if(p->linkList.current->link->parent != NULL) {
                    fprintf(backupFile, "%d;", p->linkList.current->link->parent->id);
                } else {
                    fprintf(stderr, "error : card parent of a link bad allocation\n");
                    fclose(backupFile);
                    return -1;
                }
                if(p->linkList.current->link->child != NULL) {
                    fprintf(backupFile, "%d\n", p->linkList.current->link->child->id);
                } else {
                    fprintf(stderr, "error : card child of a link bad allocation\n");
                    fclose(backupFile);
                    return -1;
                }

                setOnNextEdge(&p->linkList);
            }
        }
        fprintf(backupFile,"!\n");

        fclose(backupFile);

        return 0;

    } else {
        fprintf(stderr, "error : project bad allocation\n");
        return -1;
    }
}

/**
 * Load a project from a [projectName].txt in the /[projectPath] directory
 * The information stored has to respect a precise and specific pattern in order to be well read
 * Allocate and fill all the needed structures in order to restore the state of the project when he was saved
 * @param path the path of the directory tree of project to load
 * @param name the name of the project to load
 * @return the project filled if it's a success, NULL if not
 */
Project* loadProject(char* path, char* name){

    Project* p = NULL;
    Card* bufferParent = NULL;
    Card* bufferChild = NULL;

    char pathBackupFile[MAXPATH+1+MAXNAME+1+MAXNAME+4+1];
    FILE* backupFile = NULL;

    int lengthFormat = 34;
    char bufferFormat[lengthFormat+1];
    char bufferDot;

    int bufferRootId;

    int bufferCardId;
    int bufferCardType;
    int bufferCardNumber;
    int bufferCardFixedNumber;
    int bufferCardImage;

    int bufferLinkId;
    int bufferLinkType;
    int bufferParentId;
    int bufferChildId;

    p = allocProject();

    if(p == NULL){
        fprintf(stderr, "error : project bad allocation\n");
        return NULL;
    }

    initProjectForLoad(p, path, name);

    // construction of the backup file path

    strncpy(pathBackupFile, p->path, MAXPATH);
    strcat(pathBackupFile, "/");
    strncat(pathBackupFile, p->name, MAXNAME);
    strcat(pathBackupFile, "/");
    strncat(pathBackupFile, p->name, MAXNAME);
    strcat(pathBackupFile,".txt");

    // opening the backup file

    if((backupFile = fopen(pathBackupFile,"r")) == (FILE*) NULL){
        fprintf(stderr, "error : impossible to open the backup file\n");
        perror("msg : ");
        return NULL;
    }

    // reading the backup file and completion of a project

    // checking the format (the first line of the text file)

    fgets(bufferFormat, lengthFormat+1, backupFile);
    if(strncmp(bufferFormat, "backup file for a unlock project :", lengthFormat+1) != 0){
        fprintf(stderr, "error : backup file format is wrong\n");
        fclose(backupFile);
        return NULL;
    }

    // filling data for the project

    fscanf(backupFile, "%d;%d;%d;%d\n", &bufferRootId, &p->backImage, &p->topImage, &p->bottomImage);

    // filling data for each card

    fscanf(backupFile, "%c\n", &bufferDot);

    while(bufferDot == '.'){

        fscanf(backupFile, "%d;%d;%d;%d;%d\n",
               &bufferCardId, &bufferCardType, &bufferCardNumber,&bufferCardFixedNumber, &bufferCardImage);

        addCardForLoad(p,bufferCardId, bufferCardType, bufferCardNumber,
                           bufferCardFixedNumber, bufferCardImage);

        fscanf(backupFile, "%c\n", &bufferDot);
    }

    // setting root card of the project

    setOnFirstVertex(&p->cardList);
    while(!isOutOfListVertex(&p->cardList)){
        if(p->cardList.current->card->id == bufferRootId){
            p->root = p->cardList.current->card;
            break;
        }
        setOnNextVertex(&p->cardList);
    }

    // filling data for each link

    fscanf(backupFile, "%c\n", &bufferDot);

    while(bufferDot == '.'){

        fscanf(backupFile, "%d;%d;%d;%d\n", &bufferLinkId, &bufferLinkType, &bufferParentId, &bufferChildId);

        setOnFirstVertex(&p->cardList);
        while(!isOutOfListVertex(&p->cardList)){
            if(p->cardList.current->card->id == bufferParentId){
                bufferParent = p->cardList.current->card;
            } else if(p->cardList.current->card->id == bufferChildId){
                bufferChild = p->cardList.current->card;
            }
            setOnNextVertex(&p->cardList);
        }

        addLinkForLoad(p, bufferLinkId, bufferLinkType, bufferParent, bufferChild);

        fscanf(backupFile, "%c\n", &bufferDot);
    }

    // closing the backup file

    fclose(backupFile);

    return p;
}
