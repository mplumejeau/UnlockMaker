/*
// Created by Louenn on 17/02/2023.
*/

#ifndef UNLOCKMAKER_PROJECT_H
#define UNLOCKMAKER_PROJECT_H

#include "Struct.h"

Project* allocProject(void);
void initProject(Project* p, char* path, char* name);
void freeProject(Project* p);

void setName(Project* p, char* name);
void setRoot(Project* p, Card* root);

Card* addCard(Project* p);
int deleteCard(Project* p, Card* c);

int setBackImage(Project* p, char* path);
int setTopImage(Project* p, char* path);
int setBottomImage(Project* p, char* path);

int loadProject(Project* p);
int saveProject(Project* p);
int createPrintable(Project* p);

int checkLoops(Project* p);

void runCompatibility(Project* p);
void runDiscard(Project* p);
int assignNumbers(Project* p);

#endif /*UNLOCKMAKER_PROJECT_H*/
