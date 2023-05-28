//
// Created by maxime on 19/03/23.
//

#include <stdio.h>
#include <stdlib.h>

#include <setjmp.h>
#include <cmocka.h>

#include "VertexTestsU.h"
#include "VertexListTestsU.h"
#include "EdgeTestsU.h"
#include "EdgeListTestsU.h"
#include "LinkTestsU.h"
#include "ProjectTestsU.h"
#include "AlgoTestsU.h"

//main

int main(void){

    const struct CMUnitTest testsVertex[] =
            {
                    cmocka_unit_test_setup_teardown(testInitVertex, setupNonInitVertex, teardownNonInitVertex),
                    cmocka_unit_test_setup_teardown(testGetVertexCard, setupInitVertex, teardownInitVertex),
                    cmocka_unit_test_setup(testGetVertexCardBadAlloc, setupNonAllocVertex)
            };

    const struct CMUnitTest testsVertexList[] =
            {
                    cmocka_unit_test_setup_teardown(testInitVertexList, setupNonInitVertexList, teardownEmptyVertexList),
                    cmocka_unit_test_setup_teardown(testIsVertexListEmptyTrue, setupInitEmptyVertexList, teardownEmptyVertexList),
                    cmocka_unit_test_setup_teardown(testIsVertexListEmptyFalse, setupInitOneElementVertexList, teardownOneElementVertexList),
                    cmocka_unit_test_setup(testIsVertexListEmptyBadAlloc, setupNonAllocVertexList),
                    cmocka_unit_test_setup_teardown(testIsFirstVertexTrue,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup_teardown(testIsFirstVertexFalse,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup(testIsFirstVertexBadAlloc, setupNonAllocVertexList),
                    cmocka_unit_test_setup_teardown(testIsLastVertexTrue,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup_teardown(testIsLastVertexFalse,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup(testIsLastVertexBadAlloc, setupNonAllocVertexList),
                    cmocka_unit_test_setup_teardown(testIsOutOfVertexListTrue,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup_teardown(testIsOutOfVertexListFalse,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup(testIsOutOfVertexListBadAlloc, setupNonAllocVertexList),
                    cmocka_unit_test_setup_teardown(testSetOnNextVertex,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup_teardown(testSetOnPreviousVertex,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup_teardown(testCountVertexElements0,setupInitEmptyVertexList,teardownEmptyVertexList),
                    cmocka_unit_test_setup_teardown(testCountVertexElements1,setupInitOneElementVertexList,teardownOneElementVertexList),
                    cmocka_unit_test_setup_teardown(testCountVertexElements2,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup(testCountVertexElementsBadAlloc, setupNonAllocVertexList),
                    cmocka_unit_test_setup_teardown(testInsertVertexLast0,setupInitEmptyVertexList,teardownEmptyVertexList),
                    cmocka_unit_test_setup_teardown(testInsertVertexLast1,setupInitOneElementVertexList,teardownOneElementVertexList),
                    cmocka_unit_test_setup_teardown(testInsertVertexLast2,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup(testInsertVertexLastBadAlloc, setupNonAllocVertexList),
                    cmocka_unit_test_setup_teardown(testFindCardTrue,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup_teardown(testFindCardFalse,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup(testFindCardBadAlloc, setupNonAllocVertexList),
                    cmocka_unit_test_setup_teardown(testDeleteVertexTrue,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup_teardown(testDeleteVertexFalse,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup(testDeleteVertexBadAlloc, setupNonAllocVertexList)
            };

    const struct CMUnitTest testsEdge[] =
            {
                    cmocka_unit_test_setup_teardown(testInitEdge, setupNonInitEdge, teardownNonInitEdge),
                    cmocka_unit_test_setup_teardown(testGetEdgeLink, setupInitEdge, teardownInitEdge),
                    cmocka_unit_test_setup(testGetEdgeLinkBadAlloc, setupNonAllocEdge)
            };

    const struct CMUnitTest testsEdgeList[] =
            {
                    cmocka_unit_test_setup_teardown(testInitEdgeList, setupNonInitEdgeList, teardownEmptyEdgeList),
                    cmocka_unit_test_setup_teardown(testIsEdgeListEmptyTrue, setupInitEmptyEdgeList, teardownEmptyEdgeList),
                    cmocka_unit_test_setup_teardown(testIsEdgeListEmptyFalse, setupInitOneElementEdgeList, teardownOneElementEdgeList),
                    cmocka_unit_test_setup(testIsEdgeListEmptyBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testIsFirstEdgeTrue,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testIsFirstEdgeFalse,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testIsFirstEdgeBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testIsLastEdgeTrue,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testIsLastEdgeFalse,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testIsLastEdgeBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testIsOutOfEdgeListTrue,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testIsOutOfEdgeListFalse,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testIsOutOfEdgeListBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testSetOnNextEdge,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testSetOnPreviousEdge,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testCountEdgeElements0,setupInitEmptyEdgeList,teardownEmptyEdgeList),
                    cmocka_unit_test_setup_teardown(testCountEdgeElements1,setupInitOneElementEdgeList,teardownOneElementEdgeList),
                    cmocka_unit_test_setup_teardown(testCountEdgeElements2,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testCountEdgeElementsBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testInsertEdgeLast0,setupInitEmptyEdgeList,teardownEmptyEdgeList),
                    cmocka_unit_test_setup_teardown(testInsertEdgeLast1,setupInitOneElementEdgeList,teardownOneElementEdgeList),
                    cmocka_unit_test_setup_teardown(testInsertEdgeLast2,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testInsertEdgeLastBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testFindLinkTrue,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testFindLinkFalse,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testFindLinkBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testDeleteEdgeTrue,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testDeleteEdgeFalse,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testDeleteEdgeBadAlloc, setupNonAllocEdgeList)
            };

    const struct CMUnitTest testsLink[] =
            {
                    cmocka_unit_test_setup_teardown(testSetLinkType, setupAllocLink, teardownAllocLink),
                    cmocka_unit_test_setup_teardown(testSetParent, setupAllocLink, teardownAllocLink),
                    cmocka_unit_test_setup_teardown(testSetChild, setupAllocLink, teardownAllocLink)
            };

    const struct CMUnitTest testsProject[] =
            {
                    cmocka_unit_test_setup_teardown(testAddEmptyCard, setupInitProject, teardownInitProject),
                    cmocka_unit_test_setup_teardown(testSetCardImage, setupInitProject, teardownInitProject),
                    cmocka_unit_test_setup_teardown(testDeleteCard, setupInitProject, teardownInitProject),
                    cmocka_unit_test_setup_teardown(testAddLink, setupInitProject, teardownInitProject),
                    cmocka_unit_test_setup_teardown(testDeleteLink, setupInitProject, teardownInitProject),
                    cmocka_unit_test_setup_teardown(testDeleteLinkFromCards, setupInitProject, teardownInitProject),
                    cmocka_unit_test_setup_teardown(testSetBackImage, setupInitProject, teardownInitProject),
                    cmocka_unit_test_setup_teardown(testSetTopImage, setupInitProject, teardownInitProject),
                    cmocka_unit_test_setup_teardown(testSetBottomImage, setupInitProject, teardownInitProject)
            };

    const struct CMUnitTest testsAlgo[] =
            {
                    cmocka_unit_test_setup_teardown(testCheckLoopsWithoutLoops, setupInitProjectAlgo, teardownInitProjectAlgo),
                    cmocka_unit_test_setup_teardown(testCheckLoopsWithLoops, setupInitProjectAlgoWithLoops, teardownInitProjectAlgo),
                    cmocka_unit_test_setup_teardown(testAssignNumbersWithEverythingOk, setupInitProjectAlgo, teardownInitProjectAlgo),
            };


    cmocka_run_group_tests_name("Tests Vertex module", testsVertex, NULL, NULL);
    cmocka_run_group_tests_name("Tests VertexList module", testsVertexList, NULL, NULL);
    cmocka_run_group_tests_name("Tests Edge module", testsEdge, NULL, NULL);
    cmocka_run_group_tests_name("Tests EdgeList module", testsEdgeList, NULL, NULL);
    cmocka_run_group_tests_name("Tests Link module", testsLink, NULL, NULL);
    cmocka_run_group_tests_name("Tests Project module", testsProject, NULL, NULL);
    cmocka_run_group_tests_name("Tests Algo module", testsAlgo, NULL, NULL);


    return 0;
}
