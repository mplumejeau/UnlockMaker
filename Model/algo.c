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
            printf(" current card id : %d\n", currentCard->id);
            // Add the current card to the visited list
            insertVertexLast(&visitedCards, currentCard);


            setOnFirstEdge(&currentCard->children);

            while(!isOutOfListEdge(&currentCard->children)){

                Card* linkedCard = currentCard->children.current->link->child;

                printf(" linked card id : %d\n", linkedCard->id);


                // Check if the linked card has already been visited
                if (findCard(&visitedCards, linkedCard) != 0) {
                    // If the card has already been visited, return 1 to indicate a loop
                    return 1;
                }

                // Add the linked card to the visited list
                insertVertexLast(&visitedCards, linkedCard);

                setOnNextEdge(&currentCard->children);
            }



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
void assignNumbers(Project* p) {
        int assignedNumbers[MAXCARD] = {0}; // Tableau pour stocker les numéros déjà attribués
        int assignedNumbersIndex = 0; // Indice du tableau assignedNumbers

        // Parcours des liens pour attribuer les numéros aux parents
        setOnFirstEdge(&(p->linkList));
        while (!isOutOfListEdge(&(p->linkList))) {
            Link* currentLink = p->linkList.current->link;
            if (currentLink->type == COMBINE) {
                Card* childCard = currentLink->child;

                // Vérification des parents
                setOnFirstEdge(&(childCard->parents));
                while (!isOutOfListEdge(&(childCard->parents))) {
                    Link* parentLink = childCard->parents.current->link;
                    if (parentLink->type != COMBINE) {
                        printf("Erreur : Les parents de la carte %d doivent être des liens de type COMBINE.\n", childCard->id);
                        return;
                    }
                    assignedNumbers[assignedNumbersIndex] = parentLink->parent->number;
                    assignedNumbersIndex++;
                    setOnNextEdge(&(childCard->parents));
                }

                // Attribution du numéro au child
                int sum = 0;
                for (int i = 0; i < assignedNumbersIndex; i++) {
                    sum += assignedNumbers[i];
                }
                childCard->number = sum;

                // Réinitialisation des numéros attribués pour le prochain child
                assignedNumbersIndex = 0;
            }

            setOnNextEdge(&(p->linkList));
        }

        // Parcours de la liste des cartes pour attribuer les numéros restants
        setOnFirstVertex(&(p->cardList));
        int currentNumber = 1;
        while (!isOutOfListVertex(&(p->cardList))) {
            Card* currentCard = p->cardList.current->card;
            if (currentCard->number == -1) {
                while (assignedNumbers[assignedNumbersIndex] != 0) {
                    assignedNumbersIndex++;
                }
                currentCard->number = currentNumber + assignedNumbersIndex;
                assignedNumbersIndex++;
            }
            currentNumber++;
            setOnNextVertex(&(p->cardList));
        }
    }




