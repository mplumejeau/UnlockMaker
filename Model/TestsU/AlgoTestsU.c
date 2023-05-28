//
// Created by maxime on 28/05/23.
//

#include <stdio.h>
#include <stdlib.h>

#include <setjmp.h>
#include <cmocka.h>

#include "../algo.h"
#include "../Project.h"
#include "../VertexList.h"

#include "AlgoTestsU.h"

const char* projectPath2 = "/home/maxime/Documents";

int setupInitProjectAlgo(void** state){
    Project* p = allocProject();
    initProject(p, projectPath2, "unlockTestsU");
    Card* c0 = addEmptyCard(p);
    Card* c1 = addEmptyCard(p);
    Card* c2 = addEmptyCard(p);
    Card* c3 = addEmptyCard(p);
    Card* c4 = addEmptyCard(p);
    Card* c5 = addEmptyCard(p);
    setRoot(p,c0);
    Link* l0 = addLink(p, c0, c1, DEFAULT);
    Link* l1 = addLink(p, c0, c2, DEFAULT);
    Link* l2 = addLink(p, c2, c3, DEFAULT);
    Link* l3 = addLink(p, c2, c4, DEFAULT);
    Link* l4 = addLink(p, c1, c5, COMBINE);
    Link* l5 = addLink(p, c3, c5, COMBINE);
    if(p != NULL){
        *state = p;
        return 0;
    } else {
        return -1;
    }
}

int setupInitProjectAlgoWithLoops(void** state){
    Project* p = allocProject();
    initProject(p, projectPath2, "unlockTestsU");
    Card* c0 = addEmptyCard(p);
    Card* c1 = addEmptyCard(p);
    Card* c2 = addEmptyCard(p);
    Card* c3 = addEmptyCard(p);
    Card* c4 = addEmptyCard(p);
    Card* c5 = addEmptyCard(p);
    setRoot(p,c0);
    Link* l0 = addLink(p, c0, c1, DEFAULT);
    Link* l1 = addLink(p, c0, c2, DEFAULT);
    Link* l2 = addLink(p, c2, c3, DEFAULT);
    Link* l3 = addLink(p, c2, c4, DEFAULT);
    Link* l4 = addLink(p, c1, c5, COMBINE);
    Link* l5 = addLink(p, c3, c5, COMBINE);
    Link* l6 = addLink(p, c5, c2, DEFAULT); //the link creating the loop
    if(p != NULL){
        *state = p;
        return 0;
    } else {
        return -1;
    }
}

int teardownInitProjectAlgo(void** state){
    Project* p = (Project*) (*state);
    deleteProject(p);
    free(p);
}

void testCheckLoopsWithoutLoops(void** state){
    Project* p = (Project*) (*state);
    int res = checkLoops(p);
    assert_int_equal(0, res);
}

void testCheckLoopsWithLoops(void** state){
    Project* p = (Project*) (*state);
    int res = checkLoops(p);
    assert_int_equal(1, res);
}

void testAssignNumbersWithEverythingOk(void** state){
    Project* p = (Project*) (*state);
    //int res = assignNumbers(p);

    setOnFirstVertex(&p->cardList);
    setOnNextVertex(&p->cardList);
    Card* c1 = p->cardList.current->card;
    setOnNextVertex(&p->cardList);
    setOnNextVertex(&p->cardList);
    Card* c3 = p->cardList.current->card;
    setOnNextVertex(&p->cardList);
    setOnNextVertex(&p->cardList);
    Card* c5 = p->cardList.current->card;

    //assert_int_equal(1, res);
    assert_int_equal(c1->number+c3->number, c5->number);
}