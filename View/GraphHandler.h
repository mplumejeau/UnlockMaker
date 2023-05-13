//
// Created by louenn on 08/05/23.
//

#ifndef UNLOCKMAKER_GRAPHHANDLER_H
#define UNLOCKMAKER_GRAPHHANDLER_H

#include "../Model/Project.h"

///LA DOC !!!

void initGraphFiles(Project* p);
void deleteGraphFiles();

/**
 * Takes a text file in the format of graphviz and exports it on a png format
 * @param graphInput The input file containing the text graph data
 * @param PNGoutput The output file containing the PNG graph image
 * @return 1 if the function was successful, 0 otherwise
 */
int exportPNGGraphFromFile(char* dataInput, char* PNGOutput);

int refreshGraphPNG();

void addCardGraphData(const char* id, int type);
void addLinkGraphData(char* idParent, char* idChild, int type);
void modifyCardTypeGraphData(const char* id, int newType);
void modifyLinkTypeGraphData(const char* idParent, const char* idChild, int newType);
void removeCardGraphData(const char* id);
void removeLinkGraphData(const char* idParent, const char* idChild);
void setCardAsRootGraphData(const char* id);
void removeCardAsRootGraphData();


#endif //UNLOCKMAKER_GRAPHHANDLER_H
