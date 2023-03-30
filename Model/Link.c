//
// Created by maxime on 19/03/23.
//

#include <stdio.h>
#include <stdlib.h>

#include "Link.h"

int idLinks = 0;

/**
 * Allocate a Link in the dynamic space memory
 * Give it a unique id based on idLinks
 * Increment idLinks by 1
 * @return Link* the address of the allocated structure
 */
Link* allocLink(void){
    Link* newL = (Link*) malloc(sizeof(Link));
    if (newL != NULL){
        newL->id = idLinks;
        idLinks++;
    } else {
        fprintf(stderr, "error : link bad allocation\n");
    }
    return newL;
}

/**
 * Frees the link and everything that must be deleted in it
 * Decrement idLinks by 1
 * @param l the link to free
 */
void freeLink(Link* l){
    free(l);
    idLinks --;
}

/**
 * Set the linkType for a link
 * @param l the link
 * @param t the linkType
 */
void setLinkType(Link* l, linkType t){
    if (l != NULL){
        l->type = t;
    } else {
        fprintf(stderr, "error : link bad allocation\n");
    }
}

/**
 * Set the parent Card for a link
 * @param l the link
 * @param t the parent card
 */
void setParent(Link* l, Card* c){
    if (l != NULL){
        l->parent = c;
    } else {
        fprintf(stderr, "error : link bad allocation\n");
    }
}

/**
 * Set the child Card for a link
 * @param l the link
 * @param t the child card
 */
void setChild(Link* l, Card* c){
    if (l != NULL){
        l->child = c;
    } else {
        fprintf(stderr, "error : link bad allocation\n");
    }
}

/**
 * print the content of a link
 * @param l the link to be printed
 */
 /*
void printLink(Link* l){

}
*/
