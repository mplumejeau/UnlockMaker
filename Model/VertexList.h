/*
// Created by Louenn on 28/02/2023.
*/


#ifndef UNLOCKMAKER_VERTEXLIST_H
#define UNLOCKMAKER_VERTEXLIST_H

#include "Struct.h"

void initEmptyVertexList(VertexList *vl);

int isVertexListEmpty(VertexList *vl);
int isFirstVertex(VertexList *vl);
int isLastVertex(VertexList *vl);
int isOutOfListVertex(VertexList *vl);

void setOnFirstVertex(VertexList *vl);
void setOnLastVertex(VertexList *vl);
void setOnNextVertex(VertexList *vl);
void setOnPreviousVertex(VertexList *vl);

void printVertexList(VertexList *vl);
int countVertexElements(VertexList *vl);

int insertVertexLast(VertexList *vl, Card* c);

int findCard(VertexList *vl, Card* c);
int deleteVertex(VertexList *vl, Card* c);

#endif /*UNLOCKMAKER_VERTEXLIST_H*/
