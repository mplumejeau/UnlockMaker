//
// Created by maxime on 19/03/23.
//

#include <stdio.h>
#include <stdlib.h>

#include "Link.h"

/**
 * allocate a Link in the dynamic space memory
 * @return Link* the address of the allocated structure
 */
Link* allocLink(void){
    Link* newL = (Link*) malloc(sizeof(Link));
    return newL;
}

/**
 * Frees the link and everything that must be deleted in it (deletes the image in the folder ?)
 * @param l the link to free
 */
void freeLink(Link* l){
    free(l);
}

/**
 * print the content of a link
 * @param l the link to be printed
 */
void printLink(Link* l){

}
