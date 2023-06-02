//
// Created by maxime on 19/03/23.
//

#include <stdio.h>
#include <stdlib.h>

#include <setjmp.h>
#include <cmocka.h>

#include "../Edge.h"
#include "../Link.h"

#include "EdgeTestsU.h"


//setup

int setupNonAllocEdge(void** state){
    Edge* e = NULL;
    if(e == NULL){
        *state = e;
        return 0;
    } else {
        return -1;
    }
}

int setupNonInitEdge(void** state){
    Edge* e = allocEdge();
    if(e != NULL){
        *state = e;
        return 0;
    } else {
        return -1;
    }
}

int setupInitEdge(void** state){
    Edge* e = allocEdge();
    Link* l = allocLink();
    Edge* n = NULL;
    Edge* p = NULL;
    initEdge(e,l,n,p);
    if (e != NULL && e->link != NULL){
        *state = e;
        return 0;
    } else {
        return -1;
    }
}

//teardown

int teardownNonInitEdge(void** state){
    freeEdge(*state);
    return 0;
}

int teardownInitEdge(void** state){
    Edge* e = (Edge*) (*state);
    freeLink(e->link);
    freeEdge(e);
    return 0;
}

//test initEdge
//peut on tester le cas ou le vertex est mal alloué ?

void testInitEdge(void** state){
    Edge* e = (Edge*) (*state);
    Link* l = allocLink();
    Edge* n = NULL;
    Edge* p = NULL;
    initEdge(e,l,n,p);
    assert_ptr_equal(l,e->link);
    assert_ptr_equal(n, e->next);
    assert_ptr_equal(p, e->previous);
    freeLink(l);
}

//tests getEdgeLink

void testGetEdgeLink(void** state){
    Link* res = NULL;
    Edge* e = (Edge*) (*state);
    res = getEdgeLink(e);
    assert_ptr_equal(e->link, res);
}

void testGetEdgeLinkBadAlloc(void** state){
    Link* res;
    Edge* e = (Edge*) (*state);
    res = getEdgeLink(e);
    assert_ptr_equal(NULL, res);
}
