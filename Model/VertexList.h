/*
// Created by Louenn on 28/02/2023.
*/

#include "Link.h"
#include "Card.h"
#include "Project.h"
#include "Edge.h"
#include "Vertex.h"

#ifndef UNLOCKMAKER_VERTEXLIST_H
#define UNLOCKMAKER_VERTEXLIST_H

typedef struct { /* to contain several vertices */
    Vertex sentinelFirst;
    Vertex* current;
    Vertex sentinelLast;
} VertexList;

void initVertexList(VertexList *vl);

int isVertexListEmpty(VertexList *vl);
int isFirstVertex(VertexList *vl);
int isLastVertex(VertexList *vl);
int isOutOfListVertex(VertexList *vl);

void setOnFirstVertex(VertexList *vl);
void setOnLastVertex(VertexList *vl);
void setOnNextVertex(VertexList *vl);
void setOnPreviousVertex(VertexList *vl);
//void * getCurrentAddress (VertexList *vl);

void printVertexList(VertexList *vl);
int countVertexElements(VertexList *vl);

int insertVertexLast(VertexList *vl, Card* c);

int findCard(VertexList *vl, Card* c);
void deleteVertex(VertexList *vl, Card* c);

#endif /*UNLOCKMAKER_VERTEXLIST_H*/
