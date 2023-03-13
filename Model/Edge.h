/*
// Created by Louenn on 17/02/2023.
*/

#include "Link.h"
#include "Vertex.h"
#include "Project.h"
#include "Card.h"

#ifndef UNLOCKMAKER_EDGE_H
#define UNLOCKMAKER_EDGE_H

/**
 * \brief the structure used for created list of links
 */
typedef struct edge{
    Link* link;
    struct edge* next;
    struct edge* previous;
} Edge;

Edge* allocEdge(void);

void initEdge(Edge* e, Link* l, Edge* n, Edge* p);

Link* getEdgeLink(Edge* e);

void freeEdge(Edge* e);

#endif /*UNLOCKMAKER_EDGE_H*/
