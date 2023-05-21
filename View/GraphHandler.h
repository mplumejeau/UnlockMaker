
#ifndef UNLOCKMAKER_GRAPHHANDLER_H
#define UNLOCKMAKER_GRAPHHANDLER_H

#include "../Model/Project.h"

/* -------------------- Graph File Management -------------------- */

/**
 * This function must be called AFTER the creation of the project files.\n\n
 * It initializes the paths variables and the graph files : a Graph directory
 * in the Project's directory, the .txt data file and the corresponding .png image inside.
 */
void initGraphFiles(Project* p);

/**
 * This function must be called BEFORE the destruction of the project files.\n\n
 * It deletes the .txt and .png graph files, then the Graph directory from the Project's directory.
 * @return 0 if it's a success, -1 if not
 */
int deleteGraphFiles();

/**
 * Takes a text data file in the graphviz format and exports it in a png format
 * @param dataInput The input file containing the text graph data
 * @param PNG0utput The output file containing the PNG graph image
 * @return 0 if it's a success, -1 if not
 */
int exportGraphPNGFromFile(char* dataInput, char* PNGOutput);

/**
 * Calls the PNG export function then reloads the center image on the view
 * @return 0 if it's a success, -1 if not
 */
int refreshGraphPNG();

/* -------------------- Card Management -------------------- */

/**
 * Adds a card in the data file, with the given id and type (color).\n
 * The format is the following : node[fillcolor={color}];{id};
 * @param id the id of the new card
 * @param type the type (color) of the new card
 * @return 0 if it's a success, -1 if not
 */
int addCardGraphData(const char* id, int type);

/**
 * Modifies the card with the corresponding id in the data file, giving it a new type (color)
 * @param id the id of the card to modify
 * @param newType the type (color) of the card after the modification
 * @return 0 if it's a success, -1 if not
 */
int modifyCardTypeGraphData(const char* id, int newType);

/**
 * Set the card with the corresponding id as root in the data file, changing its shape, and removing all others roots.\n
 * A new line is therefore added, or modified if it existed, and its format is the following : {id}[shape=Msquare];
 * @param id the id of the card to set as root
 * @return 0 if it's a success, -1 if not
 */
int setCardAsRootGraphData(const char* id);

/**
 * Removes all lines setting cards as roots in the data file
 * @return 0 if it's a success, -1 if not
 */
int removeCardsAsRootGraphData();

/**
 * Removes the card with the corresponding id from the data file
 * @param id the id of the card to remove
 * @return 0 if it's a success, -1 if not
 */
int removeCardGraphData(const char* id);

/* -------------------- Link Management -------------------- */

/**
 * Adds a link in the data file, with the given parent, child, and type.\n
 * The format is the following : edge[color={color}];{parentId}->{childId};
 * @param idParent the id of the parent of the link
 * @param idChild the id of the child of the link
 * @param type the type of the link
 * @return 0 if it's a success, -1 if not
 */
int addLinkGraphData(char* idParent, char* idChild, int type);

/**
 * Modifies the link with the corresponding parent and child in the data file, giving it a new type
  @param idParent the id of the parent of the link
 * @param idChild the id of the child of the link
 * @param newType the new type of the link
 * @return 0 if it's a success, -1 if not
 */
int modifyLinkTypeGraphData(const char* idParent, const char* idChild, int newType);

/**
 * Removes the link with the corresponding parent and child from the data file
 * @param idParent the parent of the link to remove
 * @param idChild the child of the link to remove
 * @return 0 if it's a success, -1 if not
 */
int removeLinkGraphData(const char* idParent, const char* idChild);


#endif //UNLOCKMAKER_GRAPHHANDLER_H
