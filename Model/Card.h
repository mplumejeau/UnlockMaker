/*
// Created by Louenn on 17/02/2023.
*/

#include "Link.h"
#include "Vertex.h"
#include "Project.h"
#include "Edge.h"
#include "EdgeList.h"
#include "VertexList.h"

#ifndef UNLOCKMAKER_CARD_H
#define UNLOCKMAKER_CARD_H

typedef enum {GREY, BLUE, RED, GREEN} cardType;

typedef struct Card{
    int id;
    cardType type;
    //int start; /* 1 : it's the first card ; 0 : it's not */ /* useful ??*/
    char number;
    int fixedNumber;
    int image; /* 1 : this card has an image ; 0 : it has none */
    VertexList discard; /* défausse */
    VertexList compatibility;
    EdgeList parents;
    EdgeList children;
} Card;

void initCard(Card* c, int idCard);
void freeCard(Card* c); /* frees the card (deletes the image ?) */

void setCardType(Card* c, cardType t);
void setCardNumber(Card* c, int n);
void fixCardNumber(Card* c, int n); /* sets the number and enables fixedNumber */
void addCardImage(Card* c);
struct Edge* addParent(Card* c, Card* parent);
struct Edge* addChild(Card* c, Card* child);

Vertex* addDiscard(Card* c, Card* discard);
void addCompatibility(Card* c1, Card* c2); /* handles both c1's compatibility list and c2's */

void writeCardBinary(Card* c, char* file);
void loadCardBinary(Card* c, char* file); /* the card must be created and initialized first */

void findCompatibility(Card* c);

#endif /*UNLOCKMAKER_CARD_H*/
