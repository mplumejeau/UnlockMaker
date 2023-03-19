//
// Created by maxime on 19/03/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <cmocka.h>

#include "VertexTestsU.h"
#include "VertexListTestsU.h"


//main

int main(void){

    const struct CMUnitTest testsVertex[] =
            {
                    cmocka_unit_test_setup_teardown(testInitVertex, setupNonInit, teardownNonInit),
                    cmocka_unit_test_setup_teardown(testGetVertexCard, setupInit, teardownInit),
                    cmocka_unit_test_setup(testGetVertexCardNonAlloc, setupNonAlloc)
            };

    const struct CMUnitTest testsVertexList[] =
            {
                    cmocka_unit_test_setup_teardown(testInitVertexList, setupNonInit, teardownEmpty),
                    cmocka_unit_test_setup_teardown(testIsVertexListEmptyTrue, setupInitEmpty, teardownEmpty),
                    cmocka_unit_test_setup_teardown(testIsVertexListEmptyFalse, setupInitOneElement, teardownOneElement),
                    cmocka_unit_test_setup(testIsVertexListEmptyBadAlloc, setupNonAlloc),
                    cmocka_unit_test_setup_teardown(testIsFirstVertexTrue,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup_teardown(testIsFirstVertexFalse,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup(testIsFirstVertexBadAlloc, setupNonAlloc),
                    cmocka_unit_test_setup_teardown(testIsLastVertexTrue,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup_teardown(testIsLastVertexFalse,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup(testIsLastVertexBadAlloc, setupNonAlloc),
                    cmocka_unit_test_setup_teardown(testIsOutOfListTrue,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup_teardown(testIsOutOfListFalse,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup(testIsOutOfListBadAlloc, setupNonAlloc),
                    cmocka_unit_test_setup_teardown(testSetOnNextVertex,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup_teardown(testSetOnPreviousVertex,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup_teardown(testCountVertexElements0,setupInitEmpty,teardownEmpty),
                    cmocka_unit_test_setup_teardown(testCountVertexElements1,setupInitOneElement,teardownOneElement),
                    cmocka_unit_test_setup_teardown(testCountVertexElements2,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup(testCountVertexElementsBadAlloc, setupNonAlloc),
                    cmocka_unit_test_setup_teardown(testInsertVertexLast0,setupInitEmpty,teardownEmpty),
                    cmocka_unit_test_setup_teardown(testInsertVertexLast1,setupInitOneElement,teardownOneElement),
                    cmocka_unit_test_setup_teardown(testInsertVertexLast2,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup(testInsertVertexLastBadAlloc, setupNonAlloc),
                    cmocka_unit_test_setup_teardown(testFindCardTrue,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup_teardown(testFindCardFalse,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup(testFindCardBadAlloc, setupNonAlloc),
                    cmocka_unit_test_setup_teardown(testDeleteVertexTrue,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup_teardown(testDeleteVertexFalse,setupInitTwoElements,teardownTwoElements),
                    cmocka_unit_test_setup(testDeleteVertexBadAlloc, setupNonAlloc)
            };

    cmocka_run_group_tests_name("Tests Vertex module", testsVertex, NULL, NULL);
    cmocka_run_group_tests_name("Tests VertexList module", testsVertexList, NULL, NULL);

    return 0;
}
