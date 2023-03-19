//
// Created by maxime on 19/03/23.
//

#ifndef UNLOCKMAKER_VERTEXTESTSU_H
#define UNLOCKMAKER_VERTEXTESTSU_H

int setupNonAllocVertex(void** state);
int setupNonInitVertex(void** state);
int setupInitVertex(void** state);

int teardownNonInitVertex(void** state);
int teardownInitVertex(void** state);

void testInitVertex(void** state);

void testGetVertexCard(void** state);
void testGetVertexCardBadAlloc(void** state);

#endif //UNLOCKMAKER_VERTEXTESTSU_H
