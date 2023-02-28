/*
// Created by Louenn on 17/02/2023.
*/

#include "Card.h"
#include "Vertex.h"
#include "Project.h"
#include "Edge.h"

#ifndef QCM_LINK_H
#define QCM_LINK_H

typedef enum {DEFAULT, COMBINE, SEARCH, HINT} linkType;

typedef struct Link {
    int id;
    linkType type;
    Card* parent;
    Card* child;
} Link;

void initLink(Link* l, int idLink);

void setLinkType(Link* l, linkType t);
void setParent(Link* l, Card* c);
void setChild(Link* l, Card* c);


#endif /*QCM_LINK_H*/
