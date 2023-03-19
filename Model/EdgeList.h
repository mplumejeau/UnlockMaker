/*
// Created by Louenn on 28/02/2023.
*/

#include "Edge.h"
#include "Link.h"

#ifndef UNLOCKMAKER_EDGELIST_H
#define UNLOCKMAKER_EDGELIST_H

<<<<<<< HEAD
=======

>>>>>>> 7e3abd4c0726842bec901bd4deabe6810a6ce3ec
/**
 * \brief the list of links structure
 */
typedef struct {
    Edge sentinelFirst;
    Edge* current;
    Edge sentinelLast;
} EdgeList;

void initEdgeList(EdgeList *el);

int isEdgeListEmpty(EdgeList *el);
int isFirstEdge(EdgeList *el);
int isLastEdge(EdgeList *el);
int isOutOfListEdge(EdgeList *el);

void setOnFirstEdge(EdgeList *el);
void setOnLastEdge(EdgeList *el);
void setOnNextEdge(EdgeList *el);
void setOnPreviousEdge(EdgeList *el);
//void * getCurrentAddress (EdgeList *el);

void printEdgeList(EdgeList *el);
int countEdgeElements(EdgeList *el);

int insertEdgeLast(EdgeList *el, Link* l);

int findLink(EdgeList *el, Link* l);
void deleteEdge(EdgeList *el, Link* l);

#endif /*UNLOCKMAKER_EDGELIST_H*/
