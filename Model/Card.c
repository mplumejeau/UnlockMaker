//
// Created by louenn on 02/03/23.
//

#include <stdio.h>
#include <stdlib.h>

#include "Card.h"
#include "VertexList.h"
#include "EdgeList.h"

/**
 * Allocate a Card in the dynamic space memory
 * Give it a unique id based on idLinks
 * Increment idLinks by 1
 * @return Card* the address of the allocated structure
 */
Card* allocCard(void){
    Card* c = (Card*) malloc(sizeof(Card));
    return c;
}

/**
 * Initialize an empty card
 * @param c the card to initialieze
 */
void initEmptyCard(Card* c){
    if (c != NULL) {
        c->id = -1;
        c->type = GREY;
        c->number = -1;
        c->fixedNumber = 0;
        c->image = 0;
        initEmptyVertexList(&c->discard);
        initEmptyVertexList(&c->compatibility);
        initEmptyEdgeList(&c->parents);
        initEmptyEdgeList(&c->children);
    } else {
        fprintf(stderr, "error : card bad allocation\n");
    }
}

/**
 * Frees the card and everything that must be deleted in it
 * @param c the card to free
 */
void freeCard(Card* c){
    // Free the card structure itself
    free(c);
}

/**
 * Sets the type (color) of the card c to t
 * @param c the card to modify
 * @param t the new type of the card
 */

void setCardType(Card* c, cardType t){
    if (c != NULL) {
        c->type = t;
    } else {
        fprintf(stderr, "error : card bad allocation\n");
    }
}

/**
 * Sets the number of the card c to n
 * @param c the card to modify
 * @param n the new number of the card
 */

void setCardNumber(Card* c, int n){
    if (c != NULL) {
        c->number = n;
    } else {
        fprintf(stderr, "error : card bad allocation\n");
    }
}

/**
 * Sets the number of the card c to n and enables the card's parameter fixedNumber (to indicate that it is chosen by the user)
 * @param c the card to modify
 * @param n the new number of the card
 */
void fixCardNumber(Card* c, int n){
    if (c != NULL) {
        c->number = n;
        c->fixedNumber = 1;
    } else {
        fprintf(stderr, "error : card bad allocation\n");
    }
}

/**
 * Disables the card's parameter fixedNumber (to indicate that it is no longer chosen by the user)
 * @param c the card to modify
 */
void unfixCardNumber(Card* c){
    if (c != NULL) {
        c->fixedNumber = 0;
    } else {
        fprintf(stderr, "error : card bad allocation\n");
    }
}

