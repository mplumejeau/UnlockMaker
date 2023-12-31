//
// Created by maxime on 19/03/23.
//

#include <stdio.h>
#include <stdlib.h>

#include "Link.h"

/**
 * Allocate a Link in the dynamic space memory
 * Give it a unique id based on idLinks
 * @return Link* the address of the allocated structure
 */
Link* allocLink(void){
    Link* newL = (Link*) malloc(sizeof(Link));
    return newL;
}

/**
 * Initialize an empty link
 * @param l the link to initialize
 */
void initEmptyLink(Link* l){
    if (l != NULL){
        l->id = -1;
        l->type = DEFAULT;
        l->parent = NULL;
        l->child = NULL;
    } else {
        fprintf(stderr, "error : link bad allocation\n");
    }
}

/**
 * Frees the link and everything that must be deleted in it
 * @param l the link to free
 */
void freeLink(Link* l){
    free(l);
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
