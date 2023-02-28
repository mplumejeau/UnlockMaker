/*
// Created by Louenn on 17/02/2023.
*/

#include "Link.h"
#include "Vertex.h"
#include "Project.h"
#include "Card.h"

#ifndef UNLOCKMAKER_EDGE_H
#define UNLOCKMAKER_EDGE_H

typedef struct edge{
    Link* link;
    struct edge* next;
    struct edge* previous;
} Edge;

Edge* allocEdge();
void initEdge(Edge* e, Link* l);

Link* getEdgeLink(Edge* e);

void freeEdge(Edge* e);

#endif /*UNLOCKMAKER_EDGE_H*/
