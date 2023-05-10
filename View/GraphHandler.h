//
// Created by louenn on 08/05/23.
//

#ifndef UNLOCKMAKER_GRAPHHANDLER_H
#define UNLOCKMAKER_GRAPHHANDLER_H


///LA DOC !!!

void addCardGraphData(char* graphDataPath, const char* id, int type);
void addLinkGraphData(char* graphDataPath, char* idParent, char* idChild, int type);
void modifyCardTypeGraphData(char* graphDataPath, const char* id, int newType);
void modifyLinkTypeGraphData(char* graphDataPath, const char* idParent, const char* idChild, int newType);
void removeCardGraphData(char* graphDataPath, const char* id);
void removeLinkGraphData(char* graphDataPath, const char* idParent, const char* idChild);
void setCardAsRootGraphData(char* graphDataPath, const char* id);
void removeCardAsRootGraphData(char* graphDataPath);


#endif //UNLOCKMAKER_GRAPHHANDLER_H
