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
                    cmocka_unit_test_setup_teardown(testIsOutOfListTrue,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup_teardown(testIsOutOfListFalse,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup(testIsOutOfListBadAlloc, setupNonAllocVertexList),
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

    cmocka_run_group_tests_name("Tests Vertex module", testsVertex, NULL, NULL);
    cmocka_run_group_tests_name("Tests VertexList module", testsVertexList, NULL, NULL);
    cmocka_run_group_tests_name("Tests Edge module", testsEdge, NULL, NULL);

    return 0;
}
