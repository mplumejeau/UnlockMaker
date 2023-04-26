//
// Created by maxime on 19/03/23.
//

#ifndef UNLOCKMAKER_EDGELISTTESTSU_H
#define UNLOCKMAKER_EDGELISTTESTSU_H

int setupNonAllocEdgeList(void** state);
int setupNonInitEdgeList(void** state);
int setupInitEmptyEdgeList(void** state);
int setupInitOneElementEdgeList(void** state);
int setupInitTwoElementsEdgeList(void** state);

int teardownEmptyEdgeList(void** state);
int teardownOneElementEdgeList(void** state);
int teardownTwoElementsEdgeList(void** state);

void testInitEdgeList(void** state);

void testIsEdgeListEmptyTrue(void** state);
void testIsEdgeListEmptyFalse(void** state);
void testIsEdgeListEmptyBadAlloc(void** state);

void testIsFirstEdgeTrue(void** state);
void testIsFirstEdgeFalse(void** state);
void testIsFirstEdgeBadAlloc(void** state);

void testIsLastEdgeTrue(void** state);
void testIsLastEdgeFalse(void** state);
void testIsLastEdgeBadAlloc(void** state);

void testIsOutOfEdgeListTrue(void** state);
void testIsOutOfEdgeListFalse(void** state);
void testIsOutOfEdgeListBadAlloc(void** state);

void testSetOnNextEdge(void** state);

void testSetOnPreviousEdge(void** state);

void testCountEdgeElements0(void** state);
void testCountEdgeElements1(void** state);
void testCountEdgeElements2(void** state);
void testCountEdgeElementsBadAlloc(void** state);

void testInsertEdgeLast0(void** state);
void testInsertEdgeLast1(void** state);
void testInsertEdgeLast2(void** state);
void testInsertEdgeLastBadAlloc(void** state);

void testFindLinkTrue(void** state);
void testFindLinkFalse(void** state);
void testFindLinkBadAlloc(void** state);

void testDeleteEdgeTrue(void** state);
void testDeleteEdgeFalse(void** state);
void testDeleteEdgeBadAlloc(void** state);

#endif //UNLOCKMAKER_EDGELISTTESTSU_H
