/*
// Created by Louenn on 17/02/2023.
*/

#ifndef UNLOCKMAKER_EDGE_H
#define UNLOCKMAKER_EDGE_H

#include "Struct.h"

Edge* allocEdge(void);

void initEdge(Edge* e, Link* l, Edge* n, Edge* p);

Link* getEdgeLink(Edge* e);

void freeEdge(Edge* e);

#endif /*UNLOCKMAKER_EDGE_H*/
