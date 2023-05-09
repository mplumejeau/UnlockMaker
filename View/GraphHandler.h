//
// Created by louenn on 08/05/23.
//

#ifndef UNLOCKMAKER_GRAPHHANDLER_H
#define UNLOCKMAKER_GRAPHHANDLER_H


///LA DOC !!!

void addCardGraphData(const char* id, int type);
void addLinkGraphData(char* idParent, char* idChild, int type);
void modifyCardTypeGraphData(const char* id, int newType);
void modifyLinkTypeGraphData(const char* idParent, const char* idChild, int newType);
void removeCardGraphData(const char* id);
void removeLinkGraphData(const char* idParent, const char* idChild);
void setCardAsRootGraphData(const char* id);
void removeCardAsRootGraphData();


#endif //UNLOCKMAKER_GRAPHHANDLER_H
