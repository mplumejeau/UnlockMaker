//
// Created by Thomas on 07/03/23.
//

#include <stdio.h>
#include <stdlib.h>
#include "Edge.h"

/**
 * allocate an edge in the dynamic space memory
 * @return Edge* the address of the allocated structure
 */
Edge* allocEdge(void){
    Edge* newE = (Edge*) malloc(sizeof(Edge));
    return newE;
}

/**
 * initialize data for an edge
 * @param e the address of the edge to be initialized
 * @param l the address of the link pointed by the vertex
 * @param n the address of the next edge in the list
 * @param p the address of the previous edge in the list
 */
void initEdge(Edge* e, Link* l, Edge* n, Edge* p){
    if(e != NULL){
        e->link = l;
        e->next = n;
        e->previous = p;
    } else {
        fprintf(stderr, "error : edge bad allocation");
    }
}

/**
 * return the address of the link pointed by the vertex
 * @param e the address of the edge
 * @return Edge* the address of the link
 */
Link* getEdgeLink(Edge* e){
    if (e != NULL){
        return e->link;
    } else {
        fprintf(stderr, "error : edge bad allocation");
        return NULL;
    }
}

/**
 * free memory stored in an edge without freeing the link pointed
 * @param v the edge to be freed
 */
void freeEdge(Edge* e){
    free(e);
}
