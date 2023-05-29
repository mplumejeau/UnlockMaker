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


int checkLoops(Project* p)  {

        // Initialisation de la liste de vertex pour suivre les cartes visitées
        VertexList visitedCards;
        initEmptyVertexList(&visitedCards);

        if (p != NULL) {

            // Définition de la carte courante comme étant la racine du projet
            Card* currentCard = p->root;

            while (currentCard != NULL) {
                // Vérification si la carte courante a déjà été visitée
                if (findCard(&visitedCards, currentCard) != 0) {
                    // Si la carte a déjà été visitée, retourne 1 pour indiquer une boucle
                    printf("Boucle détectée pour la carte %d\n", currentCard->id);
                    return 1;
                }

                // Ajout de la carte courante à la liste des cartes visitées
                insertVertexLast(&visitedCards, currentCard);

                // Parcours des liens sortants de la carte courante
                setOnFirstEdge(&currentCard->children);
                while (!isOutOfListEdge(&currentCard->children)) {
                    Card* linkedCard = currentCard->children.current->link->child;

                    // Vérification si la carte liée a déjà été visitée
                    if (findCard(&visitedCards, linkedCard) != 0) {
                        // Si la carte a déjà été visitée, retourne 1 pour indiquer une boucle
                        printf("Boucle détectée pour la carte %d\n", linkedCard->id);
                        return 1;
                    }

                    // Ajout de la carte liée à la liste des cartes visitées
                    insertVertexLast(&visitedCards, linkedCard);

                    // Passage au lien suivant
                    setOnNextEdge(&currentCard->children);
                }

                // Passage à la carte suivante
                if (isOutOfListVertex(&visitedCards)) {
                    // Si toutes les cartes ont été visitées, sort de la boucle
                    break;
                }
                setOnNextVertex(&visitedCards);
                currentCard = (Card*)visitedCards.current->card;
            }

            // Aucune boucle trouvée, retourne 0
            printf("Aucune boucle détectée.\n");
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
            printf("Child Card ID: %d\n", childCard->id);
            setOnFirstEdge(&(childCard->parents));
            while (!isOutOfListEdge(&(childCard->parents))) {
                Link* parentLink = childCard->parents.current->link;
                printf("Parent Link ID: %d\n", parentLink->id);
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







