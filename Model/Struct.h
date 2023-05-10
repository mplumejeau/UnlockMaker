//
// Created by maxime on 19/03/23.
//

#ifndef UNLOCKMAKER_STRUCT_H
#define UNLOCKMAKER_STRUCT_H

#define MAXPATH 100
#define MAXNAME 15
#define MAXCARD 60
#define MAXLINK 200

#define TAILLEMAXID 4

/**
 * An enumeration for the different types/colors of cards
 * GREY :
 * BLUE :
 * RED :
 * GREEN :
 */
typedef enum {GREY, BLUE, RED, GREEN} cardType;

/**
 *  An enumeration for the different types of links
 *  DEFAULT :
 *  COMBINE :
 *  SEARCH :
 *  HINT :
 */
typedef enum {DEFAULT, COMBINE, FIXED, HINT} linkType;

typedef struct Card Card;
typedef struct Link Link;

/**
 * A structure used for created list of cards
 * @var card : a pointer towards the card contained by the vertex
 * @var next : a pointer towards the next vertex in the list
 * @var previous : a pointer towards the previous vertex in the list
 */
typedef struct Vertex{
    Card* card;
    struct Vertex* next;
    struct Vertex* previous;
} Vertex;

/**
 * A list of cards structure
 * @var sentinelFirst : an empty vertex at the beginning of the list
 * @var current : a pointer towards the current vertex of the list (useful for browsing the list)
 * @var sentinelLast : an empty vertex at the end of the list
 */
typedef struct { /* to contain several vertices */
    Vertex sentinelFirst;
    Vertex* current;
    Vertex sentinelLast;
} VertexList;

/**
 * A structure used for created list of links
 * @var link : a pointer towards the link contained by the edge
 * @var next : a pointer towards the next edge in the list
 * @var previous : a pointer towards the previous edge in the list
 */
typedef struct Edge{
    Link* link;
    struct Edge* next;
    struct Edge* previous;
} Edge;

/**
 * A list of links structure
 * @var sentinelFirst : an empty edge at the beginning of the list
 * @var current : a pointer towards the current edge of the list (useful for browsing the list)
 * @var sentinelLast : an empty edge at the end of the list
 */
typedef struct {
    Edge sentinelFirst;
    Edge* current;
    Edge sentinelLast;
} EdgeList;

/**
 * A structure representing the card of a project, containing all the useful information it can contain
 * @var id : the unique id of the card (determined with the total number of cards, be careful when a card is deleted though)
 * @var type : the type/color of the card (see enum cardType at the top of this file for more information)
 * @var number : the number of the card in the unlock scenario (almost always different from the id)
 * @var fixedNumber : an enable to indicate if the number of this card has been chosen by the user
 *                    1 : the number of the card has been chosen
 *                    0 : it hasn't been chosen yet
 * @var image : a boolean about the presence of a front image for this card
 *                  1 : a front image for this card is present in a specific directory with the precise appropriate name
 *                  0 : no front image has been added yet for this card
 * @var discard : the card's discard list, containing pointers towards every card the player can discard when getting this one (discard = défausse)
 * @var compatibility : a list containing every card that can be present in the game at the same time as this card
 * @var parents : a list containing pointers towards every parent links of this card
 * @var children : a list containing pointers towards every child links of this card
 */
struct Card{
    int id;
    cardType type;
    int number;
    int fixedNumber;
    int image;
    VertexList discard;
    VertexList compatibility;
    EdgeList parents;
    EdgeList children;
};

/**
 * A structure representing a link between 2 cards
 * @var id : the unique id of the link (determined with the total number of cards, be careful when a card is deleted though)
 * @var type : the type of the link (see enum linkType at the top of this file for more information)
 * @var parent : the parent card of the link
 * @var child : the child car of the link, which can be discovered thanks to the parent card
 */
struct Link {
    int id;
    linkType type;
    Card* parent;
    Card* child;
};

/**
 * A structure representing a unlock scenario project, created by a user from the software
 * @var path : the absolute path of the project directory (without the name of the project)
 * @var name : the name of the project
 * @var root : the root card of the unlock scenario (the unique face up card at the beginning of the game)
 * @var cardList : the list of all the cards of the project (the order have nothing to do with the order of discovering during the game)
 * @var nbCards : the number of cards of the project
 * @var idCards : array to manage the id for each cards of the project
 *                there is a 1 in a case of this array if the index of the case is an id for an existing card and o 0 if this id is free
 * @var linkList : the list of all the links of the project
 * @var nbLinks : the number of links of the project
 * @var idLinks : array to manage the id for each links of the project
 *                there is a 1 in a case of this array if the index of the case is an id for an existing link and o 0 if this id is free
 * @var backImage : a boolean about the presence of a back image for the cards of the project
 *                  1 : a back image is present in a specific directory with the precise appropriate name
 *                  0 : no back image has been added yet
 * @var topImage : a boolean about the presence of a top image for the cards of the project
 *                  1 : a top image is present in a specific directory with the precise appropriate name
 *                  0 : no top image has been added yet
 * @var bottomImage : a boolean about the presence of a bottom image for the cards of the project
 *                  1 : a bottom image is present in a specific directory with the precise appropriate name
 *                  0 : no bottom image has been added yet
 */
typedef struct {
    char path[MAXPATH+1];
    char name[MAXNAME+1];
    Card* root; /* points on the first element of the list */
    VertexList cardList;
    int nbCards;
    int idCards[MAXCARD];
    EdgeList linkList;
    int nbLinks;
    int idLinks[MAXLINK];
    int backImage; /* 1 : project's cards have a back image ; 0 : they have none ; Images have a precise name (to be chosen) */
    int topImage; /* 1 : project's cards have a top image ; 0 : they have none ; Images have a precise name (to be chosen) */
    int bottomImage; /* 1 : project's cards have a bottom image ; 0 : they have none ; Images have a precise name (to be chosen) */
} Project;

#endif //UNLOCKMAKER_STRUCT_H
