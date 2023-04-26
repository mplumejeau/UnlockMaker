//
// Created by maxime on 19/03/23.
//

#ifndef UNLOCKMAKER_VERTEXLISTTESTSU_H
#define UNLOCKMAKER_VERTEXLISTTESTSU_H

int setupNonAllocVertexList(void** state);
int setupNonInitVertexList(void** state);
int setupInitEmptyVertexList(void** state);
int setupInitOneElementVertexList(void** state);
int setupInitTwoElementsVertexList(void** state);

int teardownEmptyVertexList(void** state);
int teardownOneElementVertexList(void** state);
int teardownTwoElementsVertexList(void** state);

void testInitVertexList(void** state);

void testIsVertexListEmptyTrue(void** state);
void testIsVertexListEmptyFalse(void** state);
void testIsVertexListEmptyBadAlloc(void** state);

void testIsFirstVertexTrue(void** state);
void testIsFirstVertexFalse(void** state);
void testIsFirstVertexBadAlloc(void** state);

void testIsLastVertexTrue(void** state);
void testIsLastVertexFalse(void** state);
void testIsLastVertexBadAlloc(void** state);

void testIsOutOfVertexListTrue(void** state);
void testIsOutOfVertexListFalse(void** state);
void testIsOutOfVertexListBadAlloc(void** state);

void testSetOnNextVertex(void** state);

void testSetOnPreviousVertex(void** state);

void testCountVertexElements0(void** state);
void testCountVertexElements1(void** state);
void testCountVertexElements2(void** state);
void testCountVertexElementsBadAlloc(void** state);

void testInsertVertexLast0(void** state);
void testInsertVertexLast1(void** state);
void testInsertVertexLast2(void** state);
void testInsertVertexLastBadAlloc(void** state);

void testFindCardTrue(void** state);
void testFindCardFalse(void** state);
void testFindCardBadAlloc(void** state);

void testDeleteVertexTrue(void** state);
void testDeleteVertexFalse(void** state);
void testDeleteVertexBadAlloc(void** state);


#endif //UNLOCKMAKER_VERTEXLISTTESTSU_H
