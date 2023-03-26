/*
// Created by Louenn on 17/02/2023.
*/

#ifndef UNLOCKMAKER_CARD_H
#define UNLOCKMAKER_CARD_H

#include "Struct.h"

Card* allocCard(void);
void initCard(Card* c, int idCard);
void freeCard(Card* c);

void setCardType(Card* c, cardType t);
void setCardNumber(Card* c, int n);
void fixCardNumber(Card* c, int n);
void unfixCardNumber(Card* c);
void addCardImage(Card* c);
void removeCardImage(Card* c);

Link* addParent(Card* c, Card* parent);
Link* addChild(Card* c, Card* child);
void removeParent(Card* c, Card* parent);
void removeChild(Card* c, Card* child);

Vertex* addDiscard(Card* c, Card* discard);
void addCompatibility(Card* c1, Card* c2);

void writeCardBinary(Card* c, char* file);
void loadCardBinary(Card* c, char* file);

void findCompatibility(Card* c);

void printCard(Card* c);

#endif /*UNLOCKMAKER_CARD_H*/
