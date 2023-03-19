//
// Created by maxime on 04/03/23.
//

#include <stdio.h>
#include <stdlib.h>

#include "VertexList.h"
#include "Vertex.h"
#include "Card.h"

/**
 * initialize an empty list of vertex and set the current pointer on sentinelFirst
 * @param vl the list to be initialized
 */
void initEmptyVertexList(VertexList *vl){
    if (vl != NULL){
        vl->sentinelFirst.card = NULL;
        vl->sentinelFirst.next = &vl->sentinelLast;
        vl->sentinelFirst.previous = NULL;

        vl->current = &vl->sentinelFirst;

        vl->sentinelLast.card = NULL;
        vl->sentinelLast.next = NULL;
        vl->sentinelLast.previous = &vl->sentinelFirst;
    } else {
        fprintf(stderr, "error : vertexList bad allocation\n");
    }
}

/**
 * return a boolean about the emptyness of a list of cards
 * @param vl the list of cards
 * @return 1 if the list is empty and 0 if the list isn't empty
 */
int isVertexListEmpty(VertexList *vl){
    if (vl != NULL){
        return vl->sentinelFirst.next == &vl->sentinelLast;
    } else {
        fprintf(stderr, "error : vertexList bad allocation\n");
        return -1;
    }
}

/**
 * return a boolean about the position of the current pointer in a list of cards
 * @param vl the list of cards
 * @return 1 if the current pointer is on the first element of the list and 0 if it isn't
 */
int isFirstVertex(VertexList *vl){
    if (vl != NULL){
        return vl->current == vl->sentinelFirst.next;
    } else {
        fprintf(stderr, "error : vertexList bad allocation\n");
        return -1;
    }
}

/**
 * return a boolean about the position of the current pointer in a list of cards
 * @param vl the list of cards
 * @return 1 if the current pointer is on the last element of the list and 0 if it isn't
 */
int isLastVertex(VertexList *vl){
    if (vl != NULL){
        return vl->current == vl->sentinelLast.previous;
    } else {
        fprintf(stderr, "error : vertexList bad allocation\n");
        return -1;
    }
}

/**
 * return a boolean about the position of the current pointer in a list of cards
 * @param vl the list of cards
 * @return 1 if the current pointer is on one of both sentinels of the list and 0 if it isn't
 */
int isOutOfListVertex(VertexList *vl){
    if (vl != NULL){
        return vl->current == &vl->sentinelLast || vl->current == &vl->sentinelFirst;
    } else {
        fprintf(stderr, "error : vertexList bad allocation\n");
        return -1;
    }
}

/**
 * set the current pointer of a list of cards on the first element of the list
 * @param vl the list of cards
 */
void setOnFirstVertex(VertexList *vl){
    if (vl != NULL){
        vl->current = vl->sentinelFirst.next;
    } else {
        fprintf(stderr, "error : vertexList bad allocation\n");
    }
}

/**
 * set the current pointer of a list of cards on the last element of the list
 * @param vl the list of cards
 */
void setOnLastVertex(VertexList *vl){
    if (vl != NULL){
        vl->current = vl->sentinelLast.previous;
    } else {
        fprintf(stderr, "error : vertexList bad allocation\n");
    }
}

/**
 * set the current pointer of a list of cards on the next element of the list
 * @param vl the list of cards
 */
void setOnNextVertex(VertexList *vl){
    if (vl != NULL && vl->current != NULL){
        vl->current = vl->current->next;
    } else {
        fprintf(stderr, "error : vertexList bad allocation or current pointer NULL\n");
    }
}

/**
 * set the current pointer of a list of cards on the previous element of the list
 * @param vl the list of cards
 */
void setOnPreviousVertex(VertexList *vl){
    if (vl != NULL && vl->current != NULL){
        vl->current = vl->current->previous;
    } else {
        fprintf(stderr, "error : vertexList bad allocation or current pointer NULL\n");
    }
}

/**
 * print the content of a list of cards
 * @param vl the list of cards
 */
void printVertexList(VertexList *vl){
    Card* c;
    if (vl != NULL){
        printf("Vertex list :\n\n");
        setOnFirstVertex(vl);
        while(!isOutOfListVertex(vl)){
            c = getVertexCard(vl->current);
            printCard(c);
            setOnNextVertex(vl);
        }
        printf("\n");
    } else {
        fprintf(stderr, "error : vertexList bad allocation\n");
    }
}

/**
 * return the number of element of a list of cards
 * @param vl the list of cards
 * @return the number of element of a list of cards
 */
int countVertexElements(VertexList *vl){
    int res = 0;
    if (vl != NULL){
        setOnFirstVertex(vl);
        while(!isOutOfListVertex(vl)){
            res ++;
            setOnNextVertex(vl);
        }
        return res;
    } else {
        fprintf(stderr, "error : vertexList bad allocation\n");
        return -1;
    }
}

/**
 * create a new vertex pointing a given card and insert it at the end of a list of cards
 * @param vl the list of cards
 * @param c the card to add in the list
 * @return 0 if it is a success, -1 if it isn't
 */
int insertVertexLast(VertexList *vl, Card* c){
    Vertex* v = NULL;
    if (vl != NULL){
        v = allocVertex();
        initVertex(v,c,NULL,NULL);
        if(isVertexListEmpty(vl)){
            vl->sentinelFirst.next = v;
            v->previous = &vl->sentinelFirst;
        } else {
            setOnLastVertex(vl);
            vl->current->next = v;
            v->previous = vl->current;
        }
        vl->sentinelLast.previous = v;
        v->next = &vl->sentinelLast;
        return 0;
    } else {
        fprintf(stderr, "error : vertexList bad allocation\n");
        return -1;
    }
}

/**
 * seek a given card in a list of cards
 * @param vl the list of cards
 * @param c the card to seek
 * @return 1 if the card is in the list, 0 if not
 */
int findCard(VertexList *vl, Card* c){
    if (vl != NULL){
        setOnFirstVertex(vl);
        while(!isOutOfListVertex(vl)){
            if(vl->current != NULL && vl->current->card == c){
                return 1;
            }
            setOnNextVertex(vl);
        }
        return 0;
    } else {
        fprintf(stderr, "error : vertexList bad allocation\n");
        return -1;
    }
}

/**
 * delete a vertex pointing a given card from a list of cards if it exists and free the vertex deleted memory
 * @param vl the list of cards
 * @param c the card to be deleted
 * @return 0 if it's a success, -1 if not
 */
int deleteVertex(VertexList* vl, Card* c){
    if (vl != NULL){
        if(findCard(vl,c)){
            vl->current->previous->next = vl->current->next;
            vl->current->next->previous = vl->current->previous;
            freeVertex(vl->current);
            setOnNextVertex(vl);
            return 0;
        }
        return -1;
    } else {
        fprintf(stderr, "error : vertexList bad allocation\n");
        return -1;
    }
}
