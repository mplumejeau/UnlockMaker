//
// Created by maxime on 19/03/23.
//

#include <stdio.h>
#include <stdlib.h>

#include <setjmp.h>
#include <cmocka.h>

#include "../EdgeList.h"
#include "../Edge.h"
#include "../Link.h"

#include "EdgeListTestsU.h"


//setup

int setupNonAllocEdgeList(void** state){
    EdgeList* el = NULL;
    if(el == NULL){
        *state = el;
        return 0;
    } else {
        return -1;
    }
}

int setupNonInitEdgeList(void** state){
    EdgeList* el = malloc(sizeof(EdgeList));
    if(el != NULL){
        *state = el;
        return 0;
    } else {
        return -1;
    }
}

int setupInitEmptyEdgeList(void** state){
    EdgeList* el = malloc(sizeof(EdgeList));
    initEmptyEdgeList(el);
    if (el != NULL){
        *state = el;
        return 0;
    } else {
        return -1;
    }
}

int setupInitOneElementEdgeList(void** state){
    EdgeList* el = malloc(sizeof(EdgeList));
    Link* l = allocLink();
    initLink(l);
    initEmptyEdgeList(el);
    insertEdgeLast(el,l);
    if (el != NULL && getEdgeLink(el->sentinelFirst.next) == l && getEdgeLink(el->sentinelLast.previous) == l){
        *state = el;
        return 0;
    } else {
        return -1;
    }
}

int setupInitTwoElementsEdgeList(void** state){
    EdgeList* el = malloc(sizeof(EdgeList));
    Link* l1 = allocLink();
    initLink(l1);
    Link* l2 = allocLink();
    initLink(l2);
    initEmptyEdgeList(el);
    insertEdgeLast(el,l1);
    insertEdgeLast(el, l2);
    if (el != NULL && getEdgeLink(el->sentinelFirst.next) == l1 && getEdgeLink(el->sentinelLast.previous) == l2){
        *state = el;
        return 0;
    } else {
        return -1;
    }
}


//teardown

int teardownEmptyEdgeList(void** state){
    free(*state);
    return 0;
}

int teardownOneElementEdgeList(void** state){
    EdgeList* el = (EdgeList*) (*state);
    freeLink(getEdgeLink(el->sentinelFirst.next));
    freeEdge(el->sentinelFirst.next);
    free(el);
    return 0;
}

int teardownTwoElementsEdgeList(void** state){
    EdgeList* el = (EdgeList*) (*state);
    freeLink(getEdgeLink(el->sentinelFirst.next->next));
    freeLink(getEdgeLink(el->sentinelFirst.next));
    freeEdge(el->sentinelFirst.next->next);
    freeEdge(el->sentinelFirst.next);
    free(el);
    return 0;
}


//test initEdgeList

void testInitEdgeList(void** state){
    EdgeList* el = (EdgeList*) (*state);
    initEmptyEdgeList(el);
    assert_ptr_equal(&el->sentinelLast, el->sentinelFirst.next);
    assert_ptr_equal(&el->sentinelFirst, el->sentinelLast.previous);
    assert_ptr_equal(&el->sentinelFirst, el->current);
}

//test isEdgeListEmpty

void testIsEdgeListEmptyTrue(void** state){
    EdgeList* el = (EdgeList*) (*state);
    assert_int_equal(1, isEdgeListEmpty(el));
}

void testIsEdgeListEmptyFalse(void** state){
    EdgeList* el = (EdgeList*) (*state);
    assert_int_equal(0, isEdgeListEmpty(el));
}

void testIsEdgeListEmptyBadAlloc(void** state){
    EdgeList* el = (EdgeList*) (*state);
    assert_int_equal(-1, isEdgeListEmpty(el));
}

//test isFirstEdge

void testIsFirstEdgeTrue(void** state){
    EdgeList* el = (EdgeList*) (*state);
    setOnFirstEdge(el);
    assert_int_equal(1, isFirstEdge(el));
}

void testIsFirstEdgeFalse(void** state){
    EdgeList* el = (EdgeList*) (*state);
    setOnLastEdge(el);
    assert_int_equal(0, isFirstEdge(el));
}

void testIsFirstEdgeBadAlloc(void** state){
    EdgeList* el = (EdgeList*) (*state);
    assert_int_equal(-1, isFirstEdge(el));
}

//test isLastEdge

void testIsLastEdgeTrue(void** state){
    EdgeList* el = (EdgeList*) (*state);
    setOnLastEdge(el);
    assert_int_equal(1, isLastEdge(el));
}

void testIsLastEdgeFalse(void** state){
    EdgeList* el = (EdgeList*) (*state);
    setOnFirstEdge(el);
    assert_int_equal(0, isLastEdge(el));
}

void testIsLastEdgeBadAlloc(void** state){
    EdgeList* el = (EdgeList*) (*state);
    assert_int_equal(-1, isLastEdge(el));
}

//test isOutOfList

void testIsOutOfEdgeListTrue(void** state){
    EdgeList* el = (EdgeList*) (*state);
    setOnLastEdge(el);
    setOnNextEdge(el);
    assert_int_equal(1, isOutOfListEdge(el));
}

void testIsOutOfEdgeListFalse(void** state){
    EdgeList* el = (EdgeList*) (*state);
    setOnFirstEdge(el);
    assert_int_equal(0, isOutOfListEdge(el));
}

void testIsOutOfEdgeListBadAlloc(void** state){
    EdgeList* el = (EdgeList*) (*state);
    assert_int_equal(-1, isOutOfListEdge(el));
}

//can we consider that setOnFirstEdge and setOnLastEdge are tested respectively by testIsOnFirstEdgeTrue and testIsOnLastEdgeTrue ?

//test setOnNextEdge

void testSetOnNextEdge(void** state){
    EdgeList* el = (EdgeList*) (*state);
    Edge* e1;
    Edge* e2;
    setOnFirstEdge(el);
    e1 = el->current;
    setOnNextEdge(el);
    e2 = el->current;
    assert_ptr_equal(e1->next,e2);
}

//test setOnPreviousEdge

void testSetOnPreviousEdge(void** state){
    EdgeList* el = (EdgeList*) (*state);
    Edge* e1;
    Edge* e2;
    setOnLastEdge(el);
    e1 = el->current;
    setOnPreviousEdge(el);
    e2 = el->current;
    assert_ptr_equal(e1->previous,e2);
}

//test printEdgeList ??



//test countEdgeElements

void testCountEdgeElements0(void** state){
    EdgeList* el = (EdgeList*) (*state);
    int nb = countEdgeElements(el);
    assert_int_equal(0, nb);
}

void testCountEdgeElements1(void** state){
    EdgeList* el = (EdgeList*) (*state);
    int nb = countEdgeElements(el);
    assert_int_equal(1, nb);
}

void testCountEdgeElements2(void** state){
    EdgeList* el = (EdgeList*) (*state);
    int nb = countEdgeElements(el);
    assert_int_equal(2, nb);
}

void testCountEdgeElementsBadAlloc(void** state){
    EdgeList* el = (EdgeList*) (*state);
    int nb = countEdgeElements(el);
    assert_int_equal(-1, nb);
}

//test insertEdgeLast

void testInsertEdgeLast0(void** state){
    EdgeList* el = (EdgeList*) (*state);
    Link* l = allocLink();
    initLink(l);
    int success = insertEdgeLast(el,l);
    int nb = countEdgeElements(el);
    assert_int_equal(0,success);
    assert_int_equal(1, nb);
    assert_ptr_equal(l,el->sentinelLast.previous->link);
    freeLink(l);
}

void testInsertEdgeLast1(void** state){
    EdgeList* el = (EdgeList*) (*state);
    Link* l = allocLink();
    initLink(l);
    int success = insertEdgeLast(el,l);
    int nb = countEdgeElements(el);
    assert_int_equal(0,success);
    assert_int_equal(2, nb);
    assert_ptr_equal(l,el->sentinelLast.previous->link);
    freeLink(l);
}

void testInsertEdgeLast2(void** state){
    EdgeList* el = (EdgeList*) (*state);
    Link* l = allocLink();
    initLink(l);
    int success = insertEdgeLast(el,l);
    int nb = countEdgeElements(el);
    assert_int_equal(0,success);
    assert_int_equal(3, nb);
    assert_ptr_equal(l,el->sentinelLast.previous->link);
    freeLink(l);
}

void testInsertEdgeLastBadAlloc(void** state){
    EdgeList* el = (EdgeList*) (*state);
    Link* l = allocLink();
    initLink(l);
    int success = insertEdgeLast(el,l);
    assert_int_equal(-1,success);
    freeLink(l);
}

//test findLink

void testFindLinkTrue(void** state){
    EdgeList* el = (EdgeList*) (*state);
    Link* l = allocLink();
    initLink(l);
    int found;
    insertEdgeLast(el,l);
    found = findLink(el,l);
    assert_int_equal(1,found);
    freeLink(l);
}

void testFindLinkFalse(void** state){
    EdgeList* el = (EdgeList*) (*state);
    Link* l = allocLink();
    initLink(l);
    int found;
    found = findLink(el,l);
    assert_int_equal(0,found);
    freeLink(l);
}

void testFindLinkBadAlloc(void** state){
    EdgeList* el = (EdgeList*) (*state);
    Link* l = allocLink();
    initLink(l);
    int found;
    found = findLink(el,l);
    assert_int_equal(-1,found);
    freeLink(l);
}

//test deleteLink

void testDeleteEdgeTrue(void** state){
    EdgeList* el = (EdgeList*) (*state);
    Link* l = allocLink();
    initLink(l);
    int success;
    insertEdgeLast(el,l);
    success = deleteEdge(el,l);
    assert_int_equal(0,success);
    freeLink(l);
}

void testDeleteEdgeFalse(void** state){
    EdgeList* el = (EdgeList*) (*state);
    Link* l = allocLink();
    initLink(l);
    int success;
    success = deleteEdge(el,l);
    assert_int_equal(-1,success);
    freeLink(l);
}

void testDeleteEdgeBadAlloc(void** state){
    EdgeList* el = (EdgeList*) (*state);
    Link* l = allocLink();
    initLink(l);
    int success;
    success = deleteEdge(el,l);
    assert_int_equal(-1,success);
    freeLink(l);
}
