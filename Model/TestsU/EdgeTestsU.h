//
// Created by maxime on 19/03/23.
//

#ifndef UNLOCKMAKER_EDGETESTSU_H
#define UNLOCKMAKER_EDGETESTSU_H

int setupNonAllocEdge(void** state);
int setupNonInitEdge(void** state);
int setupInitEdge(void** state);

int teardownNonInitEdge(void** state);
int teardownInitEdge(void** state);

void testInitEdge(void** state);

void testGetEdgeLink(void** state);
void testGetEdgeLinkBadAlloc(void** state);

#endif //UNLOCKMAKER_EDGETESTSU_H
