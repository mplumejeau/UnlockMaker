//
// Created by maxime on 06/03/23.
//

#include <stdio.h>
#include <stdlib.h>

#include <setjmp.h>
#include <cmocka.h>

#include "../VertexList.h"
#include "../Vertex.h"
#include "../Card.h"

#include "VertexListTestsU.h"


//setup

int setupNonAllocVertexList(void** state){
    VertexList* vl = NULL;
    if(vl == NULL){
        *state = vl;
        return 0;
    } else {
        return -1;
    }
}

int setupNonInitVertexList(void** state){
    VertexList* vl = malloc(sizeof(VertexList));
    if(vl != NULL){
        *state = vl;
        return 0;
    } else {
        return -1;
    }
}

int setupInitEmptyVertexList(void** state){
    VertexList* vl = malloc(sizeof(VertexList));
    initEmptyVertexList(vl);
    if (vl != NULL){
        *state = vl;
        return 0;
    } else {
        return -1;
    }
}

int setupInitOneElementVertexList(void** state){
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

int setupInitTwoElementsVertexList(void** state){
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

int teardownEmptyVertexList(void** state){
    free(*state);
    return 0;
}

int teardownOneElementVertexList(void** state){
    VertexList* vl = (VertexList*) (*state);
    freeCard(getVertexCard(vl->sentinelFirst.next));
    freeVertex(vl->sentinelFirst.next);
    free(vl);
    return 0;
}

int teardownTwoElementsVertexList(void** state){
    VertexList* vl = (VertexList*) (*state);
    freeCard(getVertexCard(vl->sentinelFirst.next->next));
    freeCard(getVertexCard(vl->sentinelFirst.next));
    freeVertex(vl->sentinelFirst.next->next);
    freeVertex(vl->sentinelFirst.next);
    free(vl);
    return 0;
}


//test initVertexList

void testInitVertexList(void** state){
    VertexList* vl = (VertexList*) (*state);
    initEmptyVertexList(vl);
    assert_ptr_equal(&vl->sentinelLast, vl->sentinelFirst.next);
    assert_ptr_equal(&vl->sentinelFirst, vl->sentinelLast.previous);
    assert_ptr_equal(&vl->sentinelFirst, vl->current);
}

//test isVertexListEmpty

void testIsVertexListEmptyTrue(void** state){
    VertexList* vl = (VertexList*) (*state);
    assert_int_equal(1, isVertexListEmpty(vl));
}

void testIsVertexListEmptyFalse(void** state){
    VertexList* vl = (VertexList*) (*state);
    assert_int_equal(0, isVertexListEmpty(vl));
}

void testIsVertexListEmptyBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    assert_int_equal(-1, isVertexListEmpty(vl));
}

//test isFirstVertex

void testIsFirstVertexTrue(void** state){
    VertexList* vl = (VertexList*) (*state);
    setOnFirstVertex(vl);
    assert_int_equal(1, isFirstVertex(vl));
}

void testIsFirstVertexFalse(void** state){
    VertexList* vl = (VertexList*) (*state);
    setOnLastVertex(vl);
    assert_int_equal(0, isFirstVertex(vl));
}

void testIsFirstVertexBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    assert_int_equal(-1, isFirstVertex(vl));
}

//test isLastVertex

void testIsLastVertexTrue(void** state){
    VertexList* vl = (VertexList*) (*state);
    setOnLastVertex(vl);
    assert_int_equal(1, isLastVertex(vl));
}

void testIsLastVertexFalse(void** state){
    VertexList* vl = (VertexList*) (*state);
    setOnFirstVertex(vl);
    assert_int_equal(0, isLastVertex(vl));
}

void testIsLastVertexBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    assert_int_equal(-1, isLastVertex(vl));
}

//test isOutOfList

void testIsOutOfVertexListTrue(void** state){
    VertexList* vl = (VertexList*) (*state);
    setOnLastVertex(vl);
    setOnNextVertex(vl);
    assert_int_equal(1, isOutOfListVertex(vl));
}

void testIsOutOfVertexListFalse(void** state){
    VertexList* vl = (VertexList*) (*state);
    setOnFirstVertex(vl);
    assert_int_equal(0, isOutOfListVertex(vl));
}

void testIsOutOfVertexListBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    assert_int_equal(-1, isOutOfListVertex(vl));
}

//can we consider that setOnFirstVertex and setOnLastVertex are tested respectively by testIsOnFirstVertexTrue and testIsOnLastVertexTrue ?

//test setOnNextVertex

void testSetOnNextVertex(void** state){
    VertexList* vl = (VertexList*) (*state);
    Vertex* v1;
    Vertex* v2;
    setOnFirstVertex(vl);
    v1 = vl->current;
    setOnNextVertex(vl);
    v2 = vl->current;
    assert_ptr_equal(v1->next,v2);
}

//test setOnPreviousVertex

void testSetOnPreviousVertex(void** state){
    VertexList* vl = (VertexList*) (*state);
    Vertex* v1;
    Vertex* v2;
    setOnLastVertex(vl);
    v1 = vl->current;
    setOnPreviousVertex(vl);
    v2 = vl->current;
    assert_ptr_equal(v1->previous,v2);
}

//test printVertexList ??



//test countVertexElements

void testCountVertexElements0(void** state){
    VertexList* vl = (VertexList*) (*state);
    int nb = countVertexElements(vl);
    assert_int_equal(0, nb);
}

void testCountVertexElements1(void** state){
    VertexList* vl = (VertexList*) (*state);
    int nb = countVertexElements(vl);
    assert_int_equal(1, nb);
}

void testCountVertexElements2(void** state){
    VertexList* vl = (VertexList*) (*state);
    int nb = countVertexElements(vl);
    assert_int_equal(2, nb);
}

void testCountVertexElementsBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    int nb = countVertexElements(vl);
    assert_int_equal(-1, nb);
}

//test insertVertexLast

void testInsertVertexLast0(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success = insertVertexLast(vl,c);
    int nb = countVertexElements(vl);
    assert_int_equal(0,success);
    assert_int_equal(1, nb);
    assert_ptr_equal(c,vl->sentinelLast.previous->card);
    freeCard(c);
}

void testInsertVertexLast1(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success = insertVertexLast(vl,c);
    int nb = countVertexElements(vl);
    assert_int_equal(0,success);
    assert_int_equal(2, nb);
    assert_ptr_equal(c,vl->sentinelLast.previous->card);
    freeCard(c);
}

void testInsertVertexLast2(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success = insertVertexLast(vl,c);
    int nb = countVertexElements(vl);
    assert_int_equal(0,success);
    assert_int_equal(3, nb);
    assert_ptr_equal(c,vl->sentinelLast.previous->card);
    freeCard(c);
}

void testInsertVertexLastBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success = insertVertexLast(vl,c);
    assert_int_equal(-1,success);
    freeCard(c);
}

//test findCard

void testFindCardTrue(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int found;
    insertVertexLast(vl,c);
    found = findCard(vl,c);
    assert_int_equal(1,found);
    freeCard(c);
}

void testFindCardFalse(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int found;
    found = findCard(vl,c);
    assert_int_equal(0,found);
    freeCard(c);
}

void testFindCardBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int found;
    found = findCard(vl,c);
    assert_int_equal(-1,found);
    freeCard(c);
}

//test deleteCard

void testDeleteVertexTrue(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success;
    insertVertexLast(vl,c);
    success = deleteVertex(vl,c);
    assert_int_equal(0,success);
    freeCard(c);
}

void testDeleteVertexFalse(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success;
    success = deleteVertex(vl,c);
    assert_int_equal(-1,success);
    freeCard(c);
}

void testDeleteVertexBadAlloc(void** state){
    VertexList* vl = (VertexList*) (*state);
    Card* c = allocCard();
    int success;
    success = deleteVertex(vl,c);
    assert_int_equal(-1,success);
    freeCard(c);
}
