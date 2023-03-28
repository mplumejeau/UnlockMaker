/*
// Created by Louenn on 17/02/2023.
*/

#ifndef UNLOCKMAKER_VERTEX_H
#define UNLOCKMAKER_VERTEX_H

#include "Struct.h"

Vertex* allocVertex(void);

void initVertex(Vertex* v, Card* c, Vertex* n, Vertex* p);

Card* getVertexCard(Vertex* v);

void freeVertex(Vertex* v);

#endif /*UNLOCKMAKER_VERTEX_H*/
