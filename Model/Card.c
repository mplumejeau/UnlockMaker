//
// Created by louenn on 02/03/23.
//

#include <stdio.h>
#include <stdlib.h>
#include "Card.h"
#include "Link.h"
#include "Vertex.h"

/**
 * allocate a Card in the dynamic space memory
 * @return Card* the address of the allocated structure
 */
Card* allocCard(){
    return NULL;
}

/**
 * Initializes a card and allocates everything it needs, and giving it its unique id
 * @param c the card to initialize
 * @param idCard the new id of the card c, found by adding one to the number of cards of the card's project
 */
void initCard(Card* c, int idCard)
{


}

/**
 * Frees the card and everything that must be deleted in it (deletes the image in the folder ?)
 * @param c the card to free
 */
void freeCard(Card* c)
{


}

/**
 * Sets the type (color) of the card c to t
 * @param c the card to modify
 * @param t the new type of the card
 */
void setCardType(Card* c, cardType t)
{


}

/**
 * Sets the number of the card c to n
 * @param c the card to modify
 * @param n the new number of the card
 */
void setCardNumber(Card* c, int n)
{


}

/**
 * Sets the number of the card c to n and enables the card's parameter fixedNumber (to indicate that it is chosen by the user)
 * @param c the card to modify
 * @param n the new number of the card
 */
void fixCardNumber(Card* c, int n)
{


}

/**
 * Disables the card's parameter fixedNumber (to indicate that it is no longer chosen by the user)
 * @param c the card to modify
 */
void unfixCardNumber(Card* c)
{


}

/**
 * Enables the card's image parameter to indicate that this card has an image in the folder (card images are named with the id of the card)
 * @param c the card to modify
 */
void addCardImage(Card* c)
{


}

/**
 * Disables the card's image parameter to indicate that this card no longer has an image associated to it in the folder
 * @param c the card to modify
 */
void removeCardImage(Card* c)
{


}

/**
 * Creates a link between the card c and a card parent, adds this link in both c's parents list and parent's children list
 * @param c the current card (the new child of the card parent)
 * @param parent the new parent of the card c
 * @return a pointer to the Link between the current card and its new parent
 */
Link* addParent(Card* c, Card* parent)
{

    return NULL;
}

/**
 * Creates a link between the card c and a card child, adds this link in both c's children list and child's parents list
 * It reuses the function addParent (because it is the same principle)
 * @param c the current card (the new parent of the card child)
 * @param child the new child of the card c
 * @return a pointer to the Link between the current card and its new child
 */
Link* addChild(Card* c, Card* child)
{

    return NULL;
}

/**
 * If it exists, deletes the link between the card c and the card parent, and remove it from both c's parents list and parent's children list
 * If it doesn't exist, this function does nothing
 * TEMPORARY COMMENT : It may need another function to search for this link in the cards' lists
 * @param c the current card (no longer the child of the card parent)
 * @param parent the former parent of the card c
 */
void removeParent(Card* c, Card* parent)
{


}

/**
 * If it exists, deletes the link between the card c and the card child, and remove it from both c's children list and child's parents list
 * If it doesn't exist, this function does nothing
 * It reuses the function removeParent (because it is the same principle)
 * @param c the current card (no longer the parent of the card child)
 * @param parent the former child of the card c
 */
void removeChild(Card* c, Card* child)
{


}

/**
 * Adds the card discard to c's discard list
 * (if it is already present, this does nothing and returns a pointer to the existing Vertex in c's list pointing to the card discard)
 * @param c the card to modify
 * @param discard the card to add to c's discard list
 * @return a pointer to the Vertex in c's discard list pointing to the card discard
 */
Vertex* addDiscard(Card* c, Card* discard)
{

    return NULL;
}

/**
 * Adds both cards in each other's compatibility list to indicate that they can exist at the same time during the game
 * Handles both c1's compatibility list and c2's
 * @param c1 the first card to modify
 * @param c2 the second card to modify
 */
void addCompatibility(Card* c1, Card* c2)
{


}

/**
 * Writes all the data of a card on a binary file in the appropriate format
 * @param c the card to write on a binary file
 * @param file the path/name of the file where the data of the card needs to be writen (file is created if it does not already exist)
 */
void writeCardBinary(Card* c, char* file)
{


}

/**
 * Loads all the data of a card from a binary file
 * The card must be created and initialized first ?
 * If the file does not exist, nothing happens ? Error ?
 * @param c the card structure where the data will be transferred
 * @param file the path/name of the file that contains the data to transfer
 */
void loadCardBinary(Card* c, char* file)
{


}

/**
 * Runs an algorithm to determine which other cards can be present at the same time as the current card
 * Calls the function addCompatibility to add every compatible card in c's compatibility list
 * @param c the current card
 */
void findCompatibility(Card* c)
{


}