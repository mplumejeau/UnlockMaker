//
// Created by maxime on 06/03/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <cmocka.h>

#include "VertexList.h"
#include "Vertex.h"
#include "Card.h"


//setup

static int setupNonAlloc(void** state){
    VertexList* vl = NULL;
    if(vl == NULL){
        *state = vl;
        return 0;
    } else {
        return -1;
    }
}

static int setupNonInit(void** state){
    VertexList* vl = malloc(sizeof(VertexList));
    if(vl != NULL){
        *state = vl;
        return 0;
    } else {
        return -1;
    }
}

static int setupInitEmpty(void** state){
    VertexList* vl = malloc(sizeof(VertexList));
    initEmptyVertexList(vl);
    if (vl != NULL){
        *state = vl;
        return 0;
    } else {
        return -1;
    }
}

static int setupInitOneElement(void** state){
    VertexList* vl = malloc(sizeof(VertexList));
    Card* c = allocCard();
    initEmptyVertexList(vl);
    insertVertexLast(vl,c);
    if (vl != NULL && getVertexCard(vl->sentinelFirst.next) == c && getVertexCard(vl->sentinelLast.previous) == c){
        *state = vl;
        return 0;
    } else {
        return -1;
    }
}

static int setupInitTwoElements(void** state){
    VertexList* vl = malloc(sizeof(VertexList));
    Card* c1 = allocCard();
    Card* c2 = allocCard();
    initEmptyVertexList(vl);
    insertVertexLast(vl,c1);
    insertVertexLast(vl, c2);
    if (vl != NULL && getVertexCard(vl->sentinelFirst.next) == c1 && getVertexCard(vl->sentinelLast.previous) == c2){
        *state = vl;
        return 0;
    } else {
        return -1;
    }
}


//teardown

static int teardownEmpty(void** state){
    free(*state);
    return 0;
}

static int teardownOneElement(void** state){
    VertexList* vl = (VertexList*) (*state);
    freeCard(getVertexCard(vl->sentinelFirst.next));
    freeVertex(vl->sentinelFirst.next);
    free(vl);
    return 0;
}

static int teardownTwoElements(void** state){
    VertexList* vl = (VertexList*) (*state);
    freeCard(getVertexCard(vl->sentinelFirst.next));
    freeCard(getVertexCard(vl->sentinelLast.previous));
    freeVertex(vl->sentinelFirst.next);
    freeVertex(vl->sentinelLast.previous);
    free(vl);
    return 0;
}


//test initVertexList

static void testInitVertexList(void** state){
    VertexList* vl = (VertexList*) (*state);
    initEmptyVertexList(vl);
    assert_int_equal(&vl->sentinelLast, vl->sentinelFirst.next);
    assert_int_equal(&vl->sentinelFirst, vl->sentinelLast.previous);
    assert_int_equal(&vl->sentinelFirst, vl->current);
}

//test isVertexListEmpty

static void testIsVertexListEmptyTrue(void** state){
    VertexList* vl = (VertexList*) (*state);
    assert_int_equal(1, isVertexListEmpty(vl));
}

static void testIsVertexListEmptyFalse(void** state){
    VertexList* vl = (VertexList*) (*state);
    assert_int_equal(0, isVertexListEmpty(vl));
}

static void testIsVertexListEmptyBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    assert_int_equal(-1, isVertexListEmpty(vl));
}

//test isFirstVertex

static void testIsFirstVertexTrue(void** state){
    VertexList* vl = (VertexList*) (*state);
    setOnFirstVertex(vl);
    assert_int_equal(1, isFirstVertex(vl));
}

static void testIsFirstVertexFalse(void** state){
    VertexList* vl = (VertexList*) (*state);
    setOnLastVertex(vl);
    assert_int_equal(0, isFirstVertex(vl));
}

static void testIsFirstVertexBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    assert_int_equal(-1, isFirstVertex(vl));
}

//test isLastVertex

static void testIsLastVertexTrue(void** state){
    VertexList* vl = (VertexList*) (*state);
    setOnLastVertex(vl);
    assert_int_equal(1, isLastVertex(vl));
}

static void testIsLastVertexFalse(void** state){
    VertexList* vl = (VertexList*) (*state);
    setOnFirstVertex(vl);
    assert_int_equal(0, isLastVertex(vl));
}

static void testIsLastVertexBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    assert_int_equal(-1, isLastVertex(vl));
}

//test isOutOfList

static void testIsOutOfListTrue(void** state){
    VertexList* vl = (VertexList*) (*state);
    setOnLastVertex(vl);
    setOnNextVertex(vl);
    assert_int_equal(1, isLastVertex(vl));
}

static void testIsOutOfListFalse(void** state){
    VertexList* vl = (VertexList*) (*state);
    setOnFirstVertex(vl);
    assert_int_equal(0, isLastVertex(vl));
}

static void testIsOutOfListBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    assert_int_equal(-1, isLastVertex(vl));
}

//can we consider that setOnFirstVertex and setOnLastVertex are tested respectively by testIsOnFirstVertexTrue and testIsOnLastVertexTrue ?

//test setOnNextVertex

static void testSetOnNextVertex(void** state){
    VertexList* vl = (VertexList*) (*state);
    Vertex* v1;
    Vertex* v2;
    setOnFirstVertex(vl);
    v1 = vl->current;
    setOnNextVertex(vl);
    v2 = vl->current;
    assert_int_equal(v1->next,v2);
}

//test setOnPreviousVertex

static void testSetOnPreviousVertex(void** state){
    VertexList* vl = (VertexList*) (*state);
    Vertex* v1;
    Vertex* v2;
    setOnLastVertex(vl);
    v1 = vl->current;
    setOnPreviousVertex(vl);
    v2 = vl->current;
    assert_int_equal(v1->previous,v2);
}

//test printVertexList ??



//test countVertexElements

static void testCountVertexElements0(void** state){
    VertexList* vl = (VertexList*) (*state);
    int nb = countVertexElements(vl);
    assert_int_equal(0, nb);
}

static void testCountVertexElements1(void** state){
    VertexList* vl = (VertexList*) (*state);
    int nb = countVertexElements(vl);
    assert_int_equal(1, nb);
}

static void testCountVertexElements2(void** state){
    VertexList* vl = (VertexList*) (*state);
    int nb = countVertexElements(vl);
    assert_int_equal(2, nb);
}

static void testCountVertexElementsBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    int nb = countVertexElements(vl);
    assert_int_equal(-1, nb);
}

//test insertVertexLast

static void testInsertVertexLast0(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success = insertVertexLast(vl,c);
    int nb = countVertexElements(vl);
    assert_int_equal(0,success);
    assert_int_equal(1, nb);
    assert_int_equal(c,vl->sentinelLast.previous->card);
    freeCard(c);
}

static void testInsertVertexLast1(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success = insertVertexLast(vl,c);
    int nb = countVertexElements(vl);
    assert_int_equal(0,success);
    assert_int_equal(2, nb);
    assert_int_equal(c,vl->sentinelLast.previous->card);
    freeCard(c);
}

static void testInsertVertexLast2(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success = insertVertexLast(vl,c);
    int nb = countVertexElements(vl);
    assert_int_equal(0,success);
    assert_int_equal(3, nb);
    assert_int_equal(c,vl->sentinelLast.previous->card);
    freeCard(c);
}

static void testInsertVertexLastBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success = insertVertexLast(vl,c);
    assert_int_equal(-1,success);
    freeCard(c);
}

//test findCard

static void testFindCardTrue(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int found;
    insertVertexLast(vl,c);
    found = findCard(vl,c);
    assert_int_equal(1,found);
    freeCard(c);
}

static void testFindCardFalse(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int found;
    found = findCard(vl,c);
    assert_int_equal(0,found);
    freeCard(c);
}

static void testFindCardBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int found;
    found = findCard(vl,c);
    assert_int_equal(-1,found);
    freeCard(c);
}

//test deleteCard

static void testDeleteVertexTrue(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success;
    insertVertexLast(vl,c);
    success = deleteVertex(vl,c);
    assert_int_equal(0,success);
    freeCard(c);
}

static void testDeleteVertexFalse(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success;
    success = deleteVertex(vl,c);
    assert_int_equal(-1,success);
    freeCard(c);
}

static void testDeleteVertexBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success;
    success = deleteVertex(vl,c);
    assert_int_equal(-1,success);
    freeCard(c);
}


//main

int main(void){

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

    return cmocka_run_group_tests_name("Tests VertexList module", testsVertexList, NULL, NULL);
}

