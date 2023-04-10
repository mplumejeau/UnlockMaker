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
 * @param p the project to initialize
 * @param path the absolute path where the project's directory has to be created
 * @param name the name of the project
 * @return 0 if it's a success, -1 if not
 */
int initProject(Project* p, char* path, char* name){

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
        initEmptyEdgeList(&p->linkList);
        p->nbLinks = 0;

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
 * Free every structures (cards, links, vertexList,...) contained by a project
 * Delete directory tree of the project
 * @param p the project to delete
 * @return 0 if it's a success, -1 if not
 */
int deleteProject(Project* p){

    char mainDirectory[MAXPATH+1+MAXNAME+1];
    char projectDirectory[MAXPATH+1+MAXNAME+1+7+1];
    char cardsDirectory[MAXPATH+1+MAXNAME+1+5+1];

    char saveFile[MAXPATH+1+MAXNAME+1+MAXNAME+4+1];
    char printFile[MAXPATH+1+MAXNAME+1+MAXNAME+4+1];

    char backImage[MAXPATH+1+MAXNAME+1+7+1+14+1];
    char topImage[MAXPATH+1+MAXNAME+1+7+1+13+1];
    char bottomImage[MAXPATH+1+MAXNAME+1+7+1+16+1];

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

        strncpy(projectDirectory, mainDirectory, strlen(mainDirectory));
        strcat(projectDirectory, "/Project");

        //third directory : [projectPath]/[projectName]/Cards

        strncpy(cardsDirectory, mainDirectory, strlen(mainDirectory));
        strcat(cardsDirectory, "/Cards");

        //project save file

        strncpy(saveFile, mainDirectory, strlen(mainDirectory));
        strcat(saveFile,"/");
        strncat(saveFile, p->name, MAXNAME);
        strcat(saveFile,".txt");

        //project printable file

        strncpy(printFile, mainDirectory, strlen(mainDirectory));
        strcat(printFile,"/");
        strncat(printFile, p->name, MAXNAME);
        strcat(printFile,".pdf");

        //project images

        strncpy(backImage, projectDirectory, strlen(projectDirectory));
        strcat(backImage, "/BackImage.jpeg");

        strncpy(topImage, projectDirectory, strlen(projectDirectory));
        strcat(topImage, "/topImage.jpeg");

        strncpy(bottomImage, projectDirectory, strlen(projectDirectory));
        strcat(bottomImage, "/bottomImage.jpeg");

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

        // remove project save and print files

        if(remove(saveFile) != 0){
            fprintf( stderr, "error : deletion of the file %s is impossible\n", saveFile);
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
            fprintf( stderr, "error : deletion of the directory %s is impossible\n", projectDirectory);
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
 * Set the name of a project
 * Modify the directory tree of the project according to the new name
 * @param p the project to rename
 * @param name the new name for the project
 */
void setName(Project* p, char* name){
    int lengthName;
    if (p != NULL ){

        lengthName = strlen(name);
        if(lengthName<MAXNAME){
            strncpy(p->name, name, lengthName);
        } else {
            strncpy(p->name, name, MAXNAME);
        }
        /*
         *
         * to finish with modify the directory tree of the project according to the new name
         *
         */

    } else {
        fprintf(stderr, "error : project bad allocation\n");
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
 * Add an empty card to a project
 * Create its directory into the directory tree of the project
 * @param p the project to which a card must be added
 * @return a pointer towards the created card
 */
Card* addEmptyCard(Project* p){
    return NULL;
}

/**
 * Remove a card from a project and free it
 * Delete its directory in the directory tree
 * @param p the project to which a card must be removed
 * @param c the card to delete
 * @return 0 if it's a success, -1 if not
 */
int deleteCard(Project* p, Card* c){
    return -1;
}

/**
 * Add a link between 2 cards of a project
 * @param p the project to which a card must be added
 * @param parent the parent card of the link
 * @param child the child card of the link
 * @return a pointer towards the link created
 */
Link* addLink(Project* p, Card* parent, Card* child, linkType type){

    if (p != NULL && parent != NULL && child != NULL){

        Link* l = allocLink();
        setParent(l, parent);
        setChild(l, child);
        setLinkType(l, type);

        insertEdgeLast(&parent->children, l);
        insertEdgeLast(&child->parents, l);

        insertEdgeLast(&p->linkList, l);
        p->nbLinks ++;

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

    if (p != NULL && l != NULL ){

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

        freeLink(l);

        return 0;

    } else {
        fprintf(stderr, "error : project or link bad allocation\n");
    }
}

/**
 * Set a back image for all cards of the project
 * Copy an image to the directory /[projectPath]/Project and rename it BackImage.jpeg
 * Set the enable backImage to 1 if it's a success
 * @param p the project
 * @param path the absolute path of the image to copy
 * @return 0 if it's a success, -1 if not
 */
int setBackImage(Project* p, char* path){
    return -1;
}

/**
 * Set a top image for all cards of the project
 * Copy an image to the directory /[projectPath]/Project and rename it TopImage.jpeg
 * Set the enable topImage to 1 if it's a success
 * @param p the project
 * @param path the absolute path of the image to copy
 * @return 0 if it's a success, -1 if not
 */
int setTopImage(Project* p, char* path){
    return -1;
}

/**
 * Set a bottom image for all cards of the project
 * Copy an image to the directory /[projectPath]/Project and rename it BottomImage.jpeg
 * Set the enable bottomImage to 1 if it's a success
 * @param p the project
 * @param path the absolute path of the image to copy
 * @return 0 if it's a success, -1 if not
 */
int setBottomImage(Project* p, char* path){
    return -1;
}

/**
 * Load a project from a [projectName].txt in the /[projectPath] directory
 * The information stored has to respect a precise and specific template in order to be well read
 * Allocate and fill all the needed structures in order to restore the state of the project when he was saved
 * @param p the project to load
 * @return 0 if it's a success, -1 if not
 */
int loadProject(Project* p){
    return -1;
}

/**
 * Save all the information about a project in a [projectName].txt in the /[projectPath] directory
 * The information stored has to respect a precise and specific template in order to be read when loading a project
 * If the .txt file doesn't exist, create it and if it already exists, erase all previous information and rewrite it
 * @param p the project to save
 * @return 0 if it's a success, -1 if not
 */
int saveProject(Project* p){
    return -1;
}

/**
 * Create a printable file containing all the formatted cards of a completed project
 * whose name is [projectName].pdf in the /[projectPath] directory
 * If the .pdf file doesn't exist, create it and if it already exists, delete it and create a new one
 * @param p the project
 * @return 0 if it's a success, -1 if not
 */
int createPrintable(Project* p){
    return -1;
}

/**
 * Run an algorithm to check that there is no loops in the graph structure of a project
 * @param p the project to check
 * @return 1 if there are loops, 0 if there isn't
 */
int checkLoops(Project* p){
    return -1;
}

/**
 * Run an algorithm to fill every card's compatibility list
 * @param p the project
 */
void runCompatibility(Project* p){

}

/**
 * Run an algorithm to fill every card's discard list
 * @param p the project
 */
void runDiscard(Project* p){

}

/**
 * Run an algorithm to automatically assign a number between 1 and 99 to every card that hasn't a number yet
 * Must take in account that several specific numbers may have been already assigned by the user
 * @param p the project
 * @return 0 if it's a success, -1 if not
 */
int assignNumbers(Project* p){
    return -1;
}
