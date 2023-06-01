/*
// Created by Louenn on 17/02/2023.
*/

#ifndef UNLOCKMAKER_PROJECT_H
#define UNLOCKMAKER_PROJECT_H

#include "Struct.h"

Project* allocProject(void);
void freeProject(Project* p);

int initProject(Project* p, char* path, char* name);
int initProjectForLoad(Project* p, char* path, char* name);
int deleteProject(Project* p);

void setRoot(Project* p, Card* root);

int assignIdCard(Project* p, Card* c);
int assignGivenIdCard(Project* p, Card* c, int id);
void unassignIdCard(Project* p, Card* c);

Card* addEmptyCard(Project* p);
Card* addCardForLoad(Project* p, int id, cardType type, int number, int fixedNumber, int image);
int setCardImage(Project* p, Card* c, char* pathImage);
int deleteCard(Project* p, Card* c);

int assignIdLink(Project* p, Link* l);
int assignGivenIdLink(Project* p, Link* l, int id);
void unassignIdLink(Project* p, Link* l);

Link* addLink(Project* p, Card* parent, Card* child, linkType type);
Link* addLinkForLoad(Project* p, int id, linkType type, Card* parent, Card* child);
int deleteLink(Project* p, Link* l);
int deleteLinkFromCards(Project* p, Card* parent, Card* child);

int setBackImage(Project* p, char* pathImage);
int setTopImage(Project* p, char* pathImage);
int setBottomImage(Project* p, char* pathImage);

int saveProject(Project* p);
Project* loadProject(char* path, char* name);

#endif /*UNLOCKMAKER_PROJECT_H*/
