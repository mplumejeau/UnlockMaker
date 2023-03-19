/*
// Created by Louenn on 28/02/2023.
*/

#ifndef UNLOCKMAKER_EDGELIST_H
#define UNLOCKMAKER_EDGELIST_H

#include "Struct.h"

void initEdgeList(EdgeList *el);

int isEdgeListEmpty(EdgeList *el);
int isFirstEdge(EdgeList *el);
int isLastEdge(EdgeList *el);
int isOutOfListEdge(EdgeList *el);

void setOnFirstEdge(EdgeList *el);
void setOnLastEdge(EdgeList *el);
void setOnNextEdge(EdgeList *el);
void setOnPreviousEdge(EdgeList *el);

void printEdgeList(EdgeList *el);
int countEdgeElements(EdgeList *el);

int insertEdgeLast(EdgeList *el, Link* l);

int findLink(EdgeList *el, Link* l);
int deleteEdge(EdgeList *el, Link* l);

#endif /*UNLOCKMAKER_EDGELIST_H*/
