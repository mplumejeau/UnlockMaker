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
 * An enumeration for the different types/colors of Cards
 */
typedef enum {GREY, BLUE, RED, GREEN} cardType;

/**
 *  An enumeration for the different types of links
 */
typedef enum {DEFAULT, COMBINE, HINT, FIXED} linkType;

typedef struct Card Card;
typedef struct Link Link;

/**
 * @struct Vertex
 * @brief A structure used to create a list of Cards
 * @var Vertex::card
 * A pointer towards the Card contained in the Vertex
 * @var Vertex::next
 * A pointer towards the next Vertex in the list
 * @var Vertex::previous
 * A pointer towards the previous Vertex in the list
 */
typedef struct Vertex{
    Card* card;
    struct Vertex* next;
    struct Vertex* previous;
} Vertex;

/**
 * @struct VertexList
 * @brief A structure containing a list of Vertexes, pointing to a Card each
 * @var VertexList::sentinelFirst
 * An empty Vertex marking the beginning of the list
 * @var VertexList::current
 * A pointer towards the current Vertex of the list (useful for browsing the list)
 * @var VertexList::sentinelLast
 * An empty Vertex marking the end of the list
 */
typedef struct { /* to contain several vertices */
    Vertex sentinelFirst;
    Vertex* current;
    Vertex sentinelLast;
} VertexList;

/**
 * @struct Edge
 * @brief A structure used to create a list of Links
 * @var Edge::link
 * A pointer towards the Link contained in the Edge
 * @var Edge::next
 * A pointer towards the next Edge in the list
 * @var Edge::previous
 * A pointer towards the previous Edge in the list
 */
typedef struct Edge{
    Link* link;
    struct Edge* next;
    struct Edge* previous;
} Edge;

/**
 * @struct EdgeList
 * @brief A structure containing a list of Edges, pointing to a Link each
 * @var EdgeList::sentinelFirst
 * An empty Edge marking the beginning of the list
 * @var EdgeList::current
 * A pointer towards the current Edge of the list (useful for browsing the list)
 * @var EdgeList::sentinelLast
 * An empty Edge marking the end of the list
 */
typedef struct {
    Edge sentinelFirst;
    Edge* current;
    Edge sentinelLast;
} EdgeList;

/**
 * @struct Card
 * @brief A structure representing the Card of a Project, containing all the useful informations regarding the attributes a Card can have
 * @var Card::id
 * The unique id of the Card
 * @var Card::type
 * The type/color of the Card (see enum cardType at the top of this file for more information)
 * @var Card::number
 * The number of the Card in the Unlock scenario (almost always different from the id)
 * @var Card::fixedNumber
 * A boolean to indicate if the number of this Card has been chosen by the user :\n
 *                    - 1 : the number of the Card has been chosen\n
 *                    - 0 : it hasn't been chosen yet
 * @var Card::image
 * A boolean about the presence of a front image for this Card :\n
 *                  - 1 : a front image for this Card is present in a specific directory with the precise appropriate name\n
 *                  - 0 : no front image has been added yet for this Card
 * @var Card::discard
 * The Card's discard list, containing pointers towards every Card the player can discard when getting this one (unused at the moment, but could be used later on)
 * @var Card::compatibility
 * A list containing every Card that can be present in the game at the same time as this Card (unused at the moment, but could be used later on)
 * @var Card::parents
 * A list containing pointers towards every parent Link of this Card
 * @var Card::children
 * A list containing pointers towards every child Link of this Card
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
 * @struct Link
 * @brief A structure representing a Link between 2 Cards
 * @var Link::id
 * The unique id of the link
 * @var Link::type
 * The type of the link (see enum linkType at the top of this file for more information)
 * @var Link::parent
 * The parent Card of the Link
 * @var Link::child
 * The child Card of the Link, which can be discovered thanks to the parent Card
 */
struct Link {
    int id;
    linkType type;
    Card* parent;
    Card* child;
};

/**
 * @struct Project
 * @brief A structure representing an Unlock scenario Project, created by a user of the software
 * @var Project::path
 * The absolute path of the Project directory (without the name of the Project)
 * @var Project::name
 * The name of the Project
 * @var Project::root
 * The root Card of the unlock scenario (the unique face up Card at the beginning of the game)
 * @var Project::cardList
 * The list of all the Cards of the Project (the order have nothing to do with the order of discovering during the game)
 * @var Project::nbCards
 * The number of Cards of the Project
 * @var Project::idCards
 * An array to manage the id for each Cards of the Project :\n
 *                - 1 in a case of this array if the index of the case is an id for an existing Card \n
 *                - 0 in a case of this array if this id is free
 * @var Project::linkList
 * The list of all the Links of the Project
 * @var Project::nbLinks
 * The number of Links of the Project
 * @var Project::idLinks
 * An array to manage the id for each Links of the Project :\n
 *                - 1 in a case of this array if the index of the case is an id for an existing Link\n
 *                - 0 in a case of this array if this id is free
 * @var Project::backImage
 * A boolean about the presence of a back image for the Cards of the Project :\n
 *                  - 1 : a back image is present in a specific directory with the precise appropriate name\n
 *                  - 0 : no back image has been added yet
 * @var Project::topImage
 * A boolean about the presence of a top image for the Cards of the Project :\n
 *                  - 1 : a top image is present in a specific directory with the precise appropriate name\n
 *                  - 0 : no top image has been added yet
 * @var Project::bottomImage
 * A boolean about the presence of a bottom image for the Cards of the Project :\n
 *                  - 1 : a bottom image is present in a specific directory with the precise appropriate name\n
 *                  - 0 : no bottom image has been added yet
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
    int backImage; /* 1 : Project's Cards have a back image ; 0 : they have none ; Images have a precise name (to be chosen) */
    int topImage; /* 1 : Project's Cards have a top image ; 0 : they have none ; Images have a precise name (to be chosen) */
    int bottomImage; /* 1 : Project's Cards have a bottom image ; 0 : they have none ; Images have a precise name (to be chosen) */
} Project;

#endif //UNLOCKMAKER_STRUCT_H
