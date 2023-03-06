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

//can we consider that setOnFirstVertex and setOnLastVertex are tested respectively by testIsOnFirstVertexTrue and testIsOnLastVertexTrue

//test setOnNext


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
                    cmocka_unit_test_setup(testIsOutOfListBadAlloc, setupNonAlloc)
            };

    return cmocka_run_group_tests_name("Tests VertexList module", testsVertexList, NULL, NULL);
}

