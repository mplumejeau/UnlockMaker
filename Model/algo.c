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

        // Initialisation
        setOnFirstVertex(&(p->cardList));
        int number = 1;

        // Parcours de la liste des cartes
        while (!isOutOfListVertex(&(p->cardList))) {
            Card* currentCard = p->cardList.current->card;

            // Vérification des parents
            setOnFirstVertex(&(currentCard->parents));
            while (!isOutOfListVertex(&(currentCard->parents))) {
                Card* parentCard = currentCard->parents.current->link->parent;
                if (parentCard->number == -1) {
                    // La carte parente n'a pas encore de numéro attribué, passe à la suivante
                    setOnNextVertex(&(currentCard->parents));
                    continue;
                }
                number += parentCard->number;  // Ajoute le numéro du parent
                setOnNextVertex(&(currentCard->parents));
            }


            // Attribue le numéro à la carte actuelle
            currentCard->number = number;

            // Passage à la carte suivante
            number++;
            setOnNextVertex(&(p->cardList));

        }
    return -1;
    }

/* parcourir le tableau de liens du projet, des que j'ai un lien combine je stock la carte child du lien dans un current card ensuite on vérifie que tous les liens parents de cette carde sont des liens combine si c'est pas le cas je revoie un -1 avec un message d'erreur
         * apres avoir verifier que c'est un lien combine onattribut des petits numéros aux paretns et je mets la somme des numéros des parents dans le child. faut stocker les numéros dèja utiliser dans un tableau.
         * apres parcourir toute la liste de carte du projet et j'attribue un numéro pas encore attribué.




