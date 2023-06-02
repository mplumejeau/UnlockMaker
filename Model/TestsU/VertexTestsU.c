//
// Created by maxime on 05/03/23.
//

#include <stdio.h>
#include <stdlib.h>

#include <setjmp.h>
#include <cmocka.h>

#include "../Vertex.h"
#include "../Card.h"

#include "VertexTestsU.h"

//setup

int setupNonAllocVertex(void** state){
    Vertex* v = NULL;
    if(v == NULL){
        *state = v;
        return 0;
    } else {
        return -1;
    }
}

int setupNonInitVertex(void** state){
    Vertex* v = allocVertex();
    if(v != NULL){
        *state = v;
        return 0;
    } else {
        return -1;
    }
}

int setupInitVertex(void** state){
    Vertex* v = allocVertex();
    Card* c = allocCard();
    Vertex* n = NULL;
    Vertex* p = NULL;
    initVertex(v,c,n,p);
    if (v != NULL && v->card != NULL){
        *state = v;
        return 0;
    } else {
        return -1;
    }
}

//teardown

int teardownNonInitVertex(void** state){
    freeVertex(*state);
    return 0;
}

int teardownInitVertex(void** state){
    Vertex* v = (Vertex*) (*state);
    freeCard(v->card);
    freeVertex(v);
    return 0;
}

//test initVertex
//peut on tester le cas ou le vertex est mal alloué ?

void testInitVertex(void** state){
    Vertex* v = (Vertex*) (*state);
    Card* c = allocCard();
    Vertex* n = NULL;
    Vertex* p = NULL;
    initVertex(v,c,n,p);
    assert_ptr_equal(c,v->card);
    assert_ptr_equal(n, v->next);
    assert_ptr_equal(p, v->previous);
    freeCard(c);
}

//tests getVertexCard

void testGetVertexCard(void** state){
    Card* res = NULL;
    Vertex* v = (Vertex*) (*state);
    res = getVertexCard(v);
    assert_ptr_equal(v->card, res);
}

void testGetVertexCardBadAlloc(void** state){
    Card* res;
    Vertex* v = (Vertex*) (*state);
    res = getVertexCard(v);
    assert_ptr_equal(NULL, res);
}
