/*
// Created by Louenn on 17/02/2023.
*/

#ifndef UNLOCKMAKER_PROJECT_H
#define UNLOCKMAKER_PROJECT_H

#include "Struct.h"

Project* allocProject(void);
void freeProject(Project* p);

int initProject(Project* p, char* path, char* name);
int deleteProject(Project* p);

void setName(Project* p, char* name);
void setRoot(Project* p, Card* root);

Card* addEmptyCard(Project* p);
int deleteCard(Project* p, Card* c);

Link* addLink(Project* p, Card* parent, Card* child, linkType type);
int deleteLink(Project* p, Link* l);

int setBackImage(Project* p, char* pathImage);
int setTopImage(Project* p, char* pathImage);
int setBottomImage(Project* p, char* pathImage);

int saveProject(Project* p);
int loadProject(Project* p);
int createPrintable(Project* p);

int checkLoops(Project* p);

void runCompatibility(Project* p);
void runDiscard(Project* p);
int assignNumbers(Project* p);

#endif /*UNLOCKMAKER_PROJECT_H*/
