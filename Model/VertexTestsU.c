//
// Created by maxime on 05/03/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <cmocka.h>

#include "Vertex.h"
#include "Card.h"

//is it possible to test allocVertex and freeVertex ?

//setup

static int setupNonAlloc(void** state){
    Vertex* v = NULL;
    if(v == NULL){
        *state = v;
        return 0;
    } else {
        return -1;
    }
}

static int setupNonInit(void** state){
    Vertex* v = allocVertex();
    if(v != NULL){
        *state = v;
        return 0;
    } else {
        return -1;
    }
}

static int setupInit(void** state){
    Vertex* v = allocVertex();
    Card* c = allocCard();
    Vertex* n = NULL;
    Vertex* p = NULL;
    initVertex(v,c,n,p);
    if (v != NULL && v->c != NULL){
        *state = v;
        return 0;
    } else {
        return -1;
    }
}

//teardown

static int teardownNonInit(void** state){
    freeVertex(*state);
    return 0;
}

static int teardownInit(void** state){
    Vertex* v = (Vertex*) (*state);
    freeCard(v->card);
    freeVertex(v);
    return 0;
}

//test initVertex
//peut on tester le cas ou le vertex est mal alloué ?

static void testInitVertex(void** state){
    Vertex* v = (Vertex*) (*state);
    Card* c = allocCard();
    Vertex* n = NULL;
    Vertex* p = NULL;
    initVertex(v,c,n,p);
    assert_int_equal(c,v->card);
    assert_int_equal(n, v->next);
    assert_int_equal(p, v->previous);
    free(c);
}

//tests getVertexCard

static void testGetVertexCard(void** state){
    Card* res;
    Vertex* v = (Vertex*) (*state);
    res = getVertexCard(v);
    assert_int_equal(v->card, res);
}

static void testGetVertexCardNonAlloc(void** state){
    Card* res;
    Vertex* v = (Vertex*) (*state);
    res = getVertexCard(v);
    assert_int_equal(NULL, res);
}

//main

int main(void){

    const struct CMUnitTest testsVertex[] =
            {
                    cmocka_unit_test_setup_teardown(testInitVertex, setupNonInit, teardownNonInit),
                    cmocka_unit_test_setup_teardown(testGetVertexCard, setupInit, teardownInit),
                    cmocka_unit_test_setup(testGetVertexCardNonAlloc, setupNonAlloc)
            };

    return cmocka_run_group_tests_name("Tests Vertex module", testsVertex, NULL, NULL);
}