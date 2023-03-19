/*
// Created by Louenn on 17/02/2023.
*/

#ifndef UNLOCKMAKER_PROJECT_H
#define UNLOCKMAKER_PROJECT_H

#include "Struct.h"

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

void runCompatibility(Project* p);
void assignNumbers(Project* p);
void runDiscard(Project* p);

void freeProject(Project* p);

#endif /*UNLOCKMAKER_PROJECT_H*/
