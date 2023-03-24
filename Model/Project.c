//
// Created by maxime on 24/03/23.
//

#include <stdio.h>
#include <stdlib.h>

#include "Project.h"

/**
 * Allocate a unlock project in the dynamic space memory
 * @return the address of the allocated structure
 */
Project* allocProject(void){
    return NULL;
}

/**
 * Initialize an empty project and create its directory tree at the specified absolute path
 * @param p the project to initialize
 * @param path the absolute path where the project's directory has to be created
 * @param name the name of the project
 */
void initProject(Project* p, char* path, char* name){

}

/**
 * Free a project and every structures (cards, links, vertexList,...) it contains
 * @param p the project to free
 */
void freeProject(Project* p){

}

/**
 * Set the name of a project
 * @param p the project to rename
 * @param name the new name for the project
 */
void setName(Project* p, char* name){

}

/**
 * Set the root card (the unique face up card at the beginning of the game) of a project
 * @param p the project whose the root has to be set
 * @param root the new root card for the project
 */
void setRoot(Project* p, Card* root){

}

/**
 * Add an empty card to a project
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
