//
// Created by miniya on 16/04/23.
//

#include <stdio.h>
#include <stddef.h>

#include "algo.h"
#include "VertexList.h"
#include "Card.h"
#include "EdgeList.h"

/**
 * Run an algorithm to check that there is no loops in the graph structure of a project
 * @param p the project to check
 * @return 1 if there are loops, 0 if there isn't
 */


int checkLoops(Project* p) {

    // Initialize the vertex list to keep track of visited cards
    VertexList visitedCards;
    initEmptyVertexList(&visitedCards);

    if (p != NULL ){

        // Set the current card to the root of the project
        Card* currentCard = p->root;

        while (currentCard != NULL) {
            // Check if the current card has already been visited
            if (findCard(&visitedCards, currentCard) != 0) {
                // If the card has already been visited, return 1 to indicate a loop
                return 1;
            }
            // Add the current card to the visited list
            insertVertexLast(&visitedCards, currentCard);

            /*
            setOnFirstEdge(&currentCard->children);

            while(!isOutOfListEdge(&currentCard->children)){

                Card* linkedCard = currentCard->children.current->link->child;

                // Check if the linked card has already been visited
                if (findCard(&visitedCards, linkedCard) != 0) {
                    // If the card has already been visited, return 1 to indicate a loop
                    return 1;
                }

                // Add the linked card to the visited list
                insertVertexLast(&visitedCards, linkedCard);

                setOnNextEdge(&currentCard->children);
            }
             */
            /*
            // Check all links of the current card
            for (int i = 0; i < currentCard->nbLinks; i++) {
                Card* linkedCard = currentCard->Link[i]->toCard;
                // Check if the linked card has already been visited
                if (findCard(&visitedCards, linkedCard) != 0) {
                    // If the card has already been visited, return 1 to indicate a loop
                    return 1;
                }
                // Add the linked card to the visited list
                insertVertexLast(&visitedCards, linkedCard);
            }
            */

            // Move to the next card
            if (isOutOfListVertex(&visitedCards)) {
                // If all cards have been visited, exit the loop
                break;
            }
            setOnNextVertex(&visitedCards);
            currentCard = (Card*)visitedCards.current->card;
        }
        // No loops were found, return 0
        return 0;

    } else {
        fprintf(stderr, "error : project bad allocation\n");
        return -1;
    }
}

/**
 * Run an algorithm to fill every card's compatibility list
 * @param p the project
 */
void runCompatibility(Project* p){

}

/**
 * Run an algorithm to fill every card's discard list
 * @param p the project
 */
void runDiscard(Project* p){

}

/**
 * Run an algorithm to automatically assign a number between 1 and 99 to every card that hasn't a number yet
 * Must take in account that several specific numbers may have been already assigned by the user
 * @param p the project
 * @return 0 if it's a success, -1 if not
 */
int assignNumbers(Project* p){
    return -1;
}
