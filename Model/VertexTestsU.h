//
// Created by maxime on 19/03/23.
//

#ifndef UNLOCKMAKER_VERTEXTESTSU_H
#define UNLOCKMAKER_VERTEXTESTSU_H

int setupNonAlloc(void** state);
int setupNonInit(void** state);
int setupInit(void** state);

int teardownNonInit(void** state);
int teardownInit(void** state);

void testInitVertex(void** state);

void testGetVertexCard(void** state);
void testGetVertexCardNonAlloc(void** state);

#endif //UNLOCKMAKER_VERTEXTESTSU_H
