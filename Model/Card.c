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

/**
 * Creates a link between the card c and a card parent, adds this link in both c's parents list and parent's children list
 * @param c the current card (the new child of the card parent)
 * @param parent the new parent of the card c
 * @return a pointer to the Link between the current card and its new parent
 */
 /*
Link* addParent(Card* c, Card* parent)
{
    // Create a new link between the current card and its new parent
    Link* link = allocLink();
    initLink(link, c, parent);

    // Add the link to the parent's children list and the current card's parents list
    addChild(parent, c);
    addLink(&(c->parents), link);

    return link;


}
*/
/**
 * Creates a link between the card c and a card child, adds this link in both c's children list and child's parents list
 * It reuses the function addParent (because it is the same principle)
 * @param c the current card (the new parent of the card child)
 * @param child the new child of the card c
 * @return a pointer to the Link between the current card and its new child
 */
 /*
Link* addChild(Card* c, Card* child)
{

    return addParent(child, c);
}
*/
/**
 * If it exists, deletes the link between the card c and the card parent, and remove it from both c's parents list and parent's children list
 * If it doesn't exist, this function does nothing
 * TEMPORARY COMMENT : It may need another function to search for this link in the cards' lists
 * @param c the current card (no longer the child of the card parent)
 * @param parent the former parent of the card c
 */
 /*
void removeParent(Card* c, Card* parent)
{
 Link* link = findLink(c->parents, parent);
    if (link == NULL) {

        return;
    }

    c->parents = removeLink(c->parents, link);

    parent->children = removeLink(parent->children, link);

    free(link);


}
*/
/**
 * If it exists, deletes the link between the card c and the card child, and remove it from both c's children list and child's parents list
 * If it doesn't exist, this function does nothing
 * It reuses the function removeParent (because it is the same principle)
 * @param c the current card (no longer the parent of the card child)
 * @param parent the former child of the card c
 */
 /*
void removeChild(Card* c, Card* child)
{
 Link* link = (Link*) malloc(sizeof(Link));
    link->parent = parent;
    link->child = c;

    c->parents = (Link**) realloc(c->parents, sizeof(Link*) * (c->parentCount + 1));
    c->parents[c->parentCount++] = link;

    parent->children = (Link**) realloc(parent->children, sizeof(Link*) * (parent->childCount + 1));
    parent->children[parent->childCount++] = link;

    return link;


}
*/
/**
 * Adds the card discard to c's discard list
 * (if it is already present, this does nothing and returns a pointer to the existing Vertex in c's list pointing to the card discard)
 * @param c the card to modify
 * @param discard the card to add to c's discard list
 * @return a pointer to the Vertex in c's discard list pointing to the card discard
 */
 /*
Vertex* addDiscard(Card* c, Card* discard)
{


        // Check if the discard card is already present in c's discard list
        for (Vertex* v = c->discardList; v != NULL; v = v->next) {
            if (v->card == discard) {
                // The discard card is already in the list, so return the existing vertex
                return v;
            }
        }

        // The discard card is not in the list, so create a new vertex for it
        Vertex* newVertex = (Vertex*) malloc(sizeof(Vertex));
        if (newVertex == NULL) {
            printf("Error: Memory allocation failed.\n");
            return NULL;
        }
        newVertex->card = discard;
        newVertex->next = NULL;
        newVertex->prev = c->discardLast;
        if (c->discardLast != NULL) {
            c->discardLast->next = newVertex;
        } else {
            c->discardList = newVertex;
        }
        c->discardLast = newVertex;

        return newVertex;
    }
*/



/**
 * Adds both cards in each other's compatibility list to indicate that they can exist at the same time during the game
 * Handles both c1's compatibility list and c2's
 * @param c1 the first card to modify
 * @param c2 the second card to modify
 */
 /*
void addCompatibility(Card* c1, Card* c2) {

        if (c == NULL || discard == NULL) {
            return NULL;
        }

        Vertex* vertex = findVertex(c->discardList, discard);

        if (vertex != NULL) {
            // discard is already in the list, return the existing vertex
            return vertex;
        }

        // discard is not in the list, add it
        return insertVertexLast(c->discardList, discard);
    }
*/



/**
 * Writes all the data of a card on a binary file in the appropriate format
 * @param c the card to write on a binary file
 * @param file the path/name of the file where the data of the card needs to be writen (file is created if it does not already exist)
 */
 /*
void writeCardBinary(Card* c, char* file)
{

        if (c == NULL || file == NULL) {
            return;
        }

        // Open the file for writing in binary mode
        FILE* f = fopen(file, "wb");

        if (f == NULL) {
            return;
        }

        // Write the card ID, type, number, and fixed status to the file
        fwrite(&(c->id), sizeof(int), 1, f);
        fwrite(&(c->type), sizeof(cardType), 1, f);
        fwrite(&(c->number), sizeof(int), 1, f);
        fwrite(&(c->isFixed), sizeof(int), 1, f);

        // Write the number of parents and children, and their IDs
        int numParents = countVertexElements(c->parentList);
        int numChildren = countVertexElements(c->childList);
        fwrite(&numParents, sizeof(int), 1, f);
        fwrite(&numChildren, sizeof(int), 1, f);

        setOnFirstVertex(c->parentList);
        while (!isOutOfListVertex(c->parentList)) {
            int parentID = ((Card*)getCurrentVertex(c->parentList))->id;
            fwrite(&parentID, sizeof(int), 1, f);
            setOnNextVertex(c->parentList);
        }

        setOnFirstVertex(c->childList);
        while (!isOutOfListVertex(c->childList)) {
            int childID = ((Card*)getCurrentVertex(c->childList))->id;
            fwrite(&childID, sizeof(int), 1, f);
            setOnNextVertex(c->childList);
        }

        // Write the number of cards in the discard list, and their IDs
        int numDiscards = countVertexElements(c->discardList);
        fwrite(&numDiscards, sizeof(int), 1, f);

        setOnFirstVertex(c->discardList);
        while (!isOutOfListVertex(c->discardList)) {
            int discardID = ((Card*)getCurrentVertex(c->discardList))->id;
            fwrite(&discardID, sizeof(int), 1, f);
            setOnNextVertex(c->discardList);
        }

        // Close the file
        fclose(f);
    }

}
*/
/**
 * Loads all the data of a card from a binary file
 * The card must be created and initialized first ?
 * If the file does not exist, nothing happens ? Error ?
 * @param c the card structure where the data will be transferred
 * @param file the path/name of the file that contains the data to transfer
 */
 /*
void loadCardBinary(Card* c, char* file)
{
    FILE* fp = fopen(file, "rb");
    if (fp == NULL) {
        printf("Error: could not open file %s for reading.\n", file);
        return;
    }

    fread(&(c->id), sizeof(int), 1, fp);
    fread(&(c->type), sizeof(cardType), 1, fp);
    fread(&(c->number), sizeof(int), 1, fp);
    fread(&(c->isFixed), sizeof(int), 1, fp);
    fread(&(c->imagePath), sizeof(char), MAX_PATH_LEN, fp);

    int numParents, numChildren, numDiscards, i;

    // Load parents
    fread(&numParents, sizeof(int), 1, fp);
    for (i = 0; i < numParents; i++) {
        int parentId;
        fread(&parentId, sizeof(int), 1, fp);
        Card* parent = getCardById(parentId);
        if (parent != NULL) {
            addParent(c, parent);
        }
    }

    // Load children
    fread(&numChildren, sizeof(int), 1, fp);
    for (i = 0; i < numChildren; i++) {
        int childId;
        fread(&childId, sizeof(int), 1, fp);
        Card* child = getCardById(childId);
        if (child != NULL) {
            addChild(c, child);
        }
    }

    // Load discards
    fread(&numDiscards, sizeof(int), 1, fp);
    for (i = 0; i < numDiscards; i++) {
        int discardId;
        fread(&discardId, sizeof(int), 1, fp);
        Card* discard = getCardById(discardId);
        if (discard != NULL) {
            addDiscard(c, discard);
        }
    }

    fclose(fp);
}

}
*/
/**
 * Runs an algorithm to determine which other cards can be present at the same time as the current card
 * Calls the function addCompatibility to add every compatible card in c's compatibility list
 * @param c the current card
 */
 /*
void findCompatibility(Card* c)
{// Clear the compatibility list
    clearVertexList(&(c->compatibilityList));

    // Loop through all other cards
    for (int i = 0; i < NUM_CARDS; i++) {
        // Check if the card is not the current card and if it is not already in the compatibility list
        if (c != &(allCards[i]) && findCard(&(c->compatibilityList), &(allCards[i])) == -1) {
            // Check if the card can be placed on top of the current card
            if (isCardCompatible(c, &(allCards[i]))) {
                // Add the current card to the other card's compatibility list and vice versa
                addCompatibility(c, &(allCards[i]));
            }
        }
    }
}


}
*/
/**
 * print the content of a card
 * @param c the card to be printed
 */
 /*
void printCard(Card* c){
    printf("Card name: %s\n", c->name);
    printf("Card type: %s\n", c->type);
    printf("Card number: %d\n", c->number);
    printf("Card color: %s\n", c->color);
    printf("Card discard list:\n");
    printVertexList(&(c->discardList));
    printf("Card compatibility list:\n");
    printVertexList(&(c->compatibilityList));
}
*/
