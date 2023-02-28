/*
// Created by Louenn on 17/02/2023.
*/

#include "Link.h"
#include "Vertex.h"
#include "Card.h"
#include "Edge.h"
#include "VertexList.h"

#ifndef UNLOCKMAKER_PROJECT_H
#define UNLOCKMAKER_PROJECT_H

#define MAXCHAR 255
#define MAXCARD 60

typedef struct {
    Card* graphBegin; /* points on the first element of the list */
    VertexList cardList;
    int nbCards;
    int nbLiens;
    char path[MAXCHAR];
    char name[MAXCHAR];
    int backImage; /* 1 : project's cards have a back image ; 0 : they have none ; Images have a precise name (to be chosen) */
    int topImage; /* 1 : project's cards have a top image ; 0 : they have none ; Images have a precise name (to be chosen) */
    int bottomImage; /* 1 : project's cards have a bottom image ; 0 : they have none ; Images have a precise name (to be chosen) */
} Project;


void initProject(Project* p);

Card* addCard(Project* p);


void deleteCard(Project* p, Card* c);

void loadProject(Project* p, char* path);
void saveProject(Project* p, char* path);
void createPrintable(Project* p);

void setBackImage(Project* p, char* path);
void setTopImage(Project* p, char* path);
void setBottomImage(Project* p, char* path);

void addName(Project* p, char* name);

int checkLoops(Project* p);

void 


void runCompatibility(Project* p);
void assignNumbers(Project* p);
void runDiscard(Project* p);

void freeProject(Project* p);

#endif /*UNLOCKMAKER_PROJECT_H*/
