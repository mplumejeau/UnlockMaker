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

/**
 * A structure representing the card of a project, containing all the useful information it can contain
 * @var id : the unique id of the card (determined with the total number of cards, be careful when a card is deleted though)
 * @var type : the type/color of the card
 * @var number : the number of the card (almost always different from the id)
 * @var fixedNumber : an enable to indicate if the number of this card was chosen by the user
 * @var image : an enable to indicate if the card has an image associated with it in the project's folder
 * @var discard : the card's discard list, containing pointers to every card the player can discard when getting this one (discard = défausse)
 * @var compatibility : a list containing every card that can be present in the game at the same time as this card
 * @var parents : a list containing pointers to every parent of this card
 * @var children : a list containing pointers to every child of this card
 */
typedef struct Card{
    int id;
    cardType type;
    char number;
    int fixedNumber;
    int image;
    VertexList discard;
    VertexList compatibility;
    EdgeList parents;
    EdgeList children;
} Card;


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

#endif /*UNLOCKMAKER_CARD_H*/
