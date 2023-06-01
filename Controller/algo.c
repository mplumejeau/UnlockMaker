//
// Created by miniya on 16/04/23.
//

#include <stdio.h>
#include <stddef.h>

#include "algo.h"
#include "../Model/VertexList.h"
#include "../Model/Card.h"
#include "../Model/EdgeList.h"

/**
 * Run an algorithm to check that there is no loops in the graph structure of a project
 * @param p the project to check
 * @return 1 if there are loops, 0 if there isn't
 */


int iterateLoops(Card* targetCard, Card* currentCard) {
    // Vérification si la carte courante est la carte cible

    // Parcours des liens sortants de la carte courante
    setOnFirstEdge(&currentCard->children);
    while (!isOutOfListEdge(&currentCard->children)) {
        Card* linkedCard = currentCard->children.current->link->child;
        printf("Target : %d ; Pointed : %d\n", targetCard->id, linkedCard->id);
        if (targetCard == linkedCard) {
            return 1; // Boucle détectée
        }
        // Appel récursif pour tester chaque carte enfant
        if (iterateLoops(targetCard, linkedCard) != 0) {
            return 1; // Boucle détectée dans la carte enfant
        }

        // Passage au lien suivant
        setOnNextEdge(&currentCard->children);
    }

    // Aucune boucle trouvée, retourne 0
    return 0;
}

/**
 * Run an algorithm to check that there is no loops in the graph structure of a project
 * @param p the project to check
 * @return 1 if there are loops, 0 if there aren't, -1 if there are errors
 */

int checkLoops(Project* p) {

    if (p != NULL) {

        Card *currentCard = NULL;

        setOnFirstVertex(&p->cardList);
        while (!isOutOfListVertex(&p->cardList)) {
            currentCard = p->cardList.current->card;
            printf("New Target : %d\n", currentCard->id);
            if (iterateLoops(currentCard, currentCard) != 0) {
                return 1;
            }
            setOnNextVertex(&p->cardList);
        }
        return 0;

    } else {
        fprintf(stderr, "Erreur : mauvaise allocation du projet.\n");
        return -1;
    }
}


/**
 * Run an algorithm to fill every card's compatibility list
 * @param p the project
 */
/*
void runCompatibility(Project* p){

}
*/
/**
 * Run an algorithm to fill every card's discard list
 * @param p the project
 */
/*
void runDiscard(Project* p){int checkLoops(Project* p) {

    // Initialisation de la liste de vertex pour suivre les cartes visitées
    VertexList visitedCards;
    initEmptyVertexList(&visitedCards);
}
*/
/**
 * Run an algorithm to automatically assign a number between 1 and 99 to every card that hasn't a number yet
 * Must take in account that several specific numbers may have been already assigned by the user
 * @param p the project
 * @return 0 if it's a success, -1 if not
 */

int assignNumbers(Project* p) {
    int assignedNumbers[MAXCARD] = {0}; // Tableau pour stocker les numéros déjà attribués
    int assignedNumbersIndex = 0; // Indice du tableau assignedNumbers

    // Parcours des liens pour attribuer les numéros aux parents
    setOnFirstEdge(&(p->linkList));
    while (!isOutOfListEdge(&(p->linkList))) {
        Link* currentLink = p->linkList.current->link;
        if (currentLink->type == COMBINE) {
            Card* childCard = currentLink->child;

            // Vérification des parents
            printf("Child Card ID: %d\n", childCard->id);
            setOnFirstEdge(&(childCard->parents));
            while (!isOutOfListEdge(&(childCard->parents))) {
                Link* parentLink = childCard->parents.current->link;
                printf("Parent Link ID: %d\n", parentLink->id);
                if (parentLink->type == COMBINE) {
                    assignedNumbers[assignedNumbersIndex] = parentLink->parent->number;
                    assignedNumbersIndex++;
                }
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
    return 0;
}







