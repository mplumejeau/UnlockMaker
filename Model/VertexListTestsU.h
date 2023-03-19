//
// Created by maxime on 19/03/23.
//

#ifndef UNLOCKMAKER_VERTEXLISTTESTSU_H
#define UNLOCKMAKER_VERTEXLISTTESTSU_H

int setupNonAlloc(void** state);
int setupNonInit(void** state);
int setupInitEmpty(void** state);
int setupInitOneElement(void** state);
int setupInitTwoElements(void** state);

int teardownEmpty(void** state);
int teardownOneElement(void** state);
int teardownTwoElements(void** state);

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

void testIsOutOfListTrue(void** state);
void testIsOutOfListFalse(void** state);
void testIsOutOfListBadAlloc(void** state);

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
