//
// Created by maxime on 19/03/23.
//

#ifndef UNLOCKMAKER_STRUCT_H
#define UNLOCKMAKER_STRUCT_H

#define MAXCHAR 255
#define MAXCARD 60

typedef enum {GREY, BLUE, RED, GREEN} cardType;
typedef enum {DEFAULT, COMBINE, SEARCH, HINT} linkType;

typedef struct Card Card;
typedef struct Link Link;

/**
 * A structure used for created list of cards
 */
typedef struct Vertex{
    Card* card;
    struct Vertex* next;
    struct Vertex* previous;
} Vertex;

/**
 * A list of links structure
 */
typedef struct { /* to contain several vertices */
    Vertex sentinelFirst;
    Vertex* current;
    Vertex sentinelLast;
} VertexList;

/**
 * A structure used for created list of links
 */
typedef struct Edge{
    Link* link;
    struct Edge* next;
    struct Edge* previous;
} Edge;

/**
 * A list of cards structure
 */
typedef struct {
    Edge sentinelFirst;
    Edge* current;
    Edge sentinelLast;
} EdgeList;

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
struct Card{
    int id;
    cardType type;
    char number;
    int fixedNumber;
    int image;
    VertexList discard;
    VertexList compatibility;
    EdgeList parents;
    EdgeList children;
};

struct Link {
    int id;
    linkType type;
    Card* parent;
    Card* child;
};

typedef struct {
    Card* graphBegin; /* points on the first element of the list */
    VertexList cardList;
    int nbCards;
    int nbLiens;
    char path[MAXCHAR];
    char name[MAXCHAR];
    int backImage; /* 1 : project's cards have a back image ; 0 : they have none ; Images have a precise name (to be chosen) */
    int topImage; /* 1 : project's cards have a top image ; 0 : they have none ; Images have a precise name (to be chosen) */
    int bottomImage; /* 1 : project's cards have a bottom image ; 0 : they have none ; Images have a precise name (to be chosen) */
} Project;

#endif //UNLOCKMAKER_STRUCT_H
