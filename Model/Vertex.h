/*
// Created by Louenn on 17/02/2023.
*/

#include "Link.h"
#include "Card.h"
#include "Project.h"
#include "Edge.h"
#include "VertexList.h"

#ifndef UNLOCKMAKER_VERTEX_H
#define UNLOCKMAKER_VERTEX_H

typedef struct Vertex{
    Card* card;
    struct Vertex* next;
    struct Vertex* previous;
} Vertex;

Vertex* allocVertex();
void initVertex(Vertex* v, Card* c);

Card* getVertexCard(Vertex* v);

void freeVertex(Vertex* v);

#endif /*UNLOCKMAKER_VERTEX_H*/
