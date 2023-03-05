//
// Created by maxime on 04/03/23.
//

#include <stdio.h>
#include <stdlib.h>
#include "Vertex.h"

/**
 * allocate a Vertex in the dynamic space memory
 * @return Vertex* the address of the allocated structure
 */
Vertex* allocVertex(){
    Vertex* newV = (Vertex*) malloc(sizeof(Vertex));
    return newV;
}

/**
 * initialize data for a vertex
 * @param v the address of the vertex to be initialized
 * @param c the address of the card pointed by the vertex
 * @param n the address of the next vertex in the list
 * @param p the address of the previous vertex in the list
 */
void initVertex(Vertex* v, Card* c, Vertex* n, Vertex* p){
    if(v != NULL){
        v->card = c;
        v->next = n;
        v->previous = p;
    } else {
        fprintf(stderr, "error : vertex bad allocation");
    }
}

/**
 * return the address of the card pointed by the vertex
 * @param v the address of the vertex
 * @return Card* the address of the card
 */
Card* getVertexCard(Vertex* v){
    if (v != NULL){
        return v->card;
    } else {
        fprintf(stderr, "error : vertex bad allocation");
        return NULL;
    }
}

/**
 * free memory stored in a vertex without freeing the card pointed
 * @param v the vertex to be freed
 */
void freeVertex(Vertex* v){
    free(v);
}