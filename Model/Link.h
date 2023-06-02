/*
// Created by Louenn on 17/02/2023.
*/

#ifndef QCM_LINK_H
#define QCM_LINK_H

#include "Struct.h"

Link* allocLink(void);
void initEmptyLink(Link* l);
void freeLink(Link* l);

void setLinkType(Link* l, linkType t);
void setParent(Link* l, Card* c);
void setChild(Link* l, Card* c);

void printLink(Link* l);


#endif /*QCM_LINK_H*/
