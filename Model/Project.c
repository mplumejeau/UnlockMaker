//
// Created by maxime on 24/03/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "Project.h"
#include "VertexList.h"

/**
 * Allocate a unlock project in the dynamic space memory
 * @return the address of the allocated structure
 */
Project* allocProject(void){
    Project* newP = (Project*) malloc(sizeof(Project));
    return newP;
}

/**
 * Initialize an empty project and create its directory tree at the specified absolute path
 * @param p the project to initialize
 * @param path the absolute path where the project's directory has to be created
 * @param name the name of the project
 */
void initProject(Project* p, char* path, char* name){

    int lengthPath, lengthName;
    char* mainDirectory;
    char* projectDirectory;

    if (p != NULL ){

        lengthPath = strlen(path);
        if(lengthPath<MAXPATH){
            // utile ??????????
            strncpy(p->path, path, lengthPath);
        } else {
            strncpy(p->path, path, MAXPATH);
        }

        lengthName = strlen(name);
        if(lengthName<MAXNAME){
            strncpy(p->name, name, lengthName);
        } else {
            strncpy(p->name, name, MAXNAME);
        }

        p->root = NULL;
        initEmptyVertexList(&p->cardList);
        p->nbCards = 0;
        p->nbLinks = 0;

        p->backImage = 0;
        p->topImage = 0;
        p->bottomImage = 0;

        // creation of the directory tree

        //first directory : [projectPath]/[projectName]

        mainDirectory = (char*) malloc((MAXPATH+1+MAXNAME)*sizeof(char));
        strncat(mainDirectory, p->path, MAXPATH);
        strncat(mainDirectory,"/",1);
        strncat(mainDirectory, p->name, MAXNAME);

        if ( mkdir(mainDirectory, 0755 ) != 0 ) {
            fprintf( stderr, "Directory %s creation is impossible\n", mainDirectory);
        }

        //second directory : [projectPath]/[projectName]/Project

        projectDirectory = (char*) malloc((MAXPATH+1+MAXNAME+1+7)*sizeof(char));
        strncat(projectDirectory, mainDirectory, MAXPATH+1+MAXNAME);
        strncat(mainDirectory, "/Project", 8);

        if ( mkdir(projectDirectory, 0755 ) != 0 ) {
            fprintf( stderr, "Directory %s creation is impossible\n", projectDirectory);
        }

    } else {
        fprintf(stderr, "error : project bad allocation\n");
    }
}

/**
 * Free a project and every structures (cards, links, vertexList,...) it contains
 * Delete directory tree of the project
 * @param p the project to free
 */
void freeProject(Project* p){

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
Card* addCard(Project* p){

}

/**
 * Remove a card from a project and free it
 * @param p the project to which a card must be removed
 * @param c the card to be deleted
 * @return 0 if it's a success, -1 if not
 */
int deleteCard(Project* p, Card* c){
    return -1;
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
