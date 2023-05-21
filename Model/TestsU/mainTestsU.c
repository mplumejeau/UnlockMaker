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
#include "EdgeListTestsU.h"
#include "LinkTestsU.h"

#include "../Project.h"
#include "../Card.h"
#include "../VertexList.h"
#include "../EdgeList.h"

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
                    cmocka_unit_test_setup_teardown(testIsOutOfVertexListTrue,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup_teardown(testIsOutOfVertexListFalse,setupInitTwoElementsVertexList,teardownTwoElementsVertexList),
                    cmocka_unit_test_setup(testIsOutOfVertexListBadAlloc, setupNonAllocVertexList),
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

    const struct CMUnitTest testsEdgeList[] =
            {
                    cmocka_unit_test_setup_teardown(testInitEdgeList, setupNonInitEdgeList, teardownEmptyEdgeList),
                    cmocka_unit_test_setup_teardown(testIsEdgeListEmptyTrue, setupInitEmptyEdgeList, teardownEmptyEdgeList),
                    cmocka_unit_test_setup_teardown(testIsEdgeListEmptyFalse, setupInitOneElementEdgeList, teardownOneElementEdgeList),
                    cmocka_unit_test_setup(testIsEdgeListEmptyBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testIsFirstEdgeTrue,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testIsFirstEdgeFalse,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testIsFirstEdgeBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testIsLastEdgeTrue,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testIsLastEdgeFalse,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testIsLastEdgeBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testIsOutOfEdgeListTrue,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testIsOutOfEdgeListFalse,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testIsOutOfEdgeListBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testSetOnNextEdge,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testSetOnPreviousEdge,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testCountEdgeElements0,setupInitEmptyEdgeList,teardownEmptyEdgeList),
                    cmocka_unit_test_setup_teardown(testCountEdgeElements1,setupInitOneElementEdgeList,teardownOneElementEdgeList),
                    cmocka_unit_test_setup_teardown(testCountEdgeElements2,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testCountEdgeElementsBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testInsertEdgeLast0,setupInitEmptyEdgeList,teardownEmptyEdgeList),
                    cmocka_unit_test_setup_teardown(testInsertEdgeLast1,setupInitOneElementEdgeList,teardownOneElementEdgeList),
                    cmocka_unit_test_setup_teardown(testInsertEdgeLast2,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testInsertEdgeLastBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testFindLinkTrue,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testFindLinkFalse,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testFindLinkBadAlloc, setupNonAllocEdgeList),
                    cmocka_unit_test_setup_teardown(testDeleteEdgeTrue,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup_teardown(testDeleteEdgeFalse,setupInitTwoElementsEdgeList,teardownTwoElementsEdgeList),
                    cmocka_unit_test_setup(testDeleteEdgeBadAlloc, setupNonAllocEdgeList)
            };

    const struct CMUnitTest testsLink[] =
            {
                    cmocka_unit_test_setup_teardown(testSetLinkType, setupAllocLink, teardownAllocLink),
                    cmocka_unit_test_setup_teardown(testSetParent, setupAllocLink, teardownAllocLink),
                    cmocka_unit_test_setup_teardown(testSetChild, setupAllocLink, teardownAllocLink)
            };


    cmocka_run_group_tests_name("Tests Vertex module", testsVertex, NULL, NULL);
    cmocka_run_group_tests_name("Tests VertexList module", testsVertexList, NULL, NULL);
    cmocka_run_group_tests_name("Tests Edge module", testsEdge, NULL, NULL);
    cmocka_run_group_tests_name("Tests EdgeList module", testsEdgeList, NULL, NULL);
    cmocka_run_group_tests_name("Tests Link module", testsLink, NULL, NULL);


    Project* p1 = NULL;
    Project* p2 = NULL;
    Project* p3 = NULL;

    Card* c1 = NULL;
    Card* c2 = NULL;
    Card* c3 = NULL;
    Card* c4 = NULL;
    Card* c5 = NULL;
    Card* c6 = NULL;
    Card* c7 = NULL;
    Card* c8 = NULL;
    Card* c9 = NULL;
    Card* c10 = NULL;
    Card* c11 = NULL;
    Card* c12 = NULL;
    Card* c13 = NULL;
    Card* c14 = NULL;
    Card* c15 = NULL;
    Card* c16 = NULL;
    Card* c17 = NULL;
    Card* c18 = NULL;
    Card* c19 = NULL;
    Card* c20 = NULL;


    Link* l1 = NULL;
    Link* l2 = NULL;
    Link* l3 = NULL;
    Link* l4 = NULL;
    Link* l5 = NULL;
    Link* l6 = NULL;

    /*
    // demo 1 : creation, completion, creation of a pdf and save of a project

    p1 = allocProject();
    initProject(p1, "/home/maxime/Documents", "unlock1");

    setBackImage(p1, "/home/maxime/Pictures/IronMan.jpg");
    setTopImage(p1, "/home/maxime/Pictures/IronMan.jpg");
    setBottomImage(p1, "/home/maxime/Pictures/IronMan.jpg");

    c1 = addEmptyCard(p1);
    setCardImage(p1,c1, "/home/maxime/Pictures/IronMan.jpg");
    //setCardImage(&p1,c1, "/home/maxime/Pictures/IronMan.jpg");

    c2 = addEmptyCard(p1);
    c3 = addEmptyCard(p1);

    deleteCard(p1, c2);
    c2 = NULL;

    c4 = addEmptyCard(p1);
    c5 = addEmptyCard(p1);

    l1 = addLink(p1, c1, c3, DEFAULT);
    l2 = addLink(p1, c1, c5, DEFAULT);
    l3 = addLink(p1, c4, c5, COMBINE);

    //l4 = addLink(p1, c5, c2, DEFAULT);

    deleteLink(p1, l2);

    deleteCard(p1, c3);

    deleteLinkFromCards(p1,c4,c5);

    saveProject(p1);

    //deleteProject(&p1);
    */

    /*
    // demo 2 : load a project from a path, modification and save it

    p2 = loadProject("/home/maxime/Documents", "unlock2");

    // attribute to c6 and c7 the card with id 2 and 3

    setOnFirstVertex(&p2->cardList);
    while(!isOutOfListVertex(&p2->cardList)){
        if(p2->cardList.current->card->id == 2){
            c6 = p2->cardList.current->card;
        }
        if(p2->cardList.current->card->id == 3){
            c7 = p2->cardList.current->card;
        }
        setOnNextVertex(&p2->cardList);
    }

    c8 = addEmptyCard(p2);

    l5 = addLink(p2, c6, c8, DEFAULT);
    l6 = addLink(p2, c7, c8, COMBINE);

    // attribute to c9 the card with id 4

    setOnFirstVertex(&p2->cardList);
    while(!isOutOfListVertex(&p2->cardList)){
        if(p2->cardList.current->card->id == 4){
            c9 = p2->cardList.current->card;
        }
        setOnNextVertex(&p2->cardList);
    }

    deleteCard(p2, c9);

    saveProject(p2);
    */


    // demo 3 : creation of several cards with images and creation of a pdf file

    p3 = allocProject();
    initProject(p3, "/home/maxime/Documents", "unlock3");

    setBackImage(p3, "/home/maxime/Pictures/DosCarte.jpg");
    setTopImage(p3, "/home/maxime/Pictures/Banniere.jpg");
    setBottomImage(p3, "/home/maxime/Pictures/TexteBasDePage.jpg");

    c10 = addEmptyCard(p3);
    c11 = addEmptyCard(p3);
    c12 = addEmptyCard(p3);
    c13 = addEmptyCard(p3);
    c14 = addEmptyCard(p3);
    c15 = addEmptyCard(p3);
    c16 = addEmptyCard(p3);
    c17 = addEmptyCard(p3);
    c18 = addEmptyCard(p3);
    c19 = addEmptyCard(p3);
    c20 = addEmptyCard(p3);

    setCardImage(p3,c10, "/home/maxime/Pictures/Chateau.jpg");
    setCardImage(p3,c11, "/home/maxime/Pictures/IronMan.jpg");
    setCardImage(p3,c12, "/home/maxime/Pictures/IronMan.jpg");
    setCardImage(p3,c13, "/home/maxime/Pictures/IronMan.jpg");
    setCardImage(p3,c14, "/home/maxime/Pictures/IronMan.jpg");
    setCardImage(p3,c15, "/home/maxime/Pictures/IronMan.jpg");
    setCardImage(p3,c16, "/home/maxime/Pictures/IronMan.jpg");
    setCardImage(p3,c17, "/home/maxime/Pictures/IronMan.jpg");
    setCardImage(p3,c16, "/home/maxime/Pictures/IronMan.jpg");
    setCardImage(p3,c18, "/home/maxime/Pictures/IronMan.jpg");
    setCardImage(p3,c19, "/home/maxime/Pictures/Chateau.jpg");
    setCardImage(p3,c20, "/home/maxime/Pictures/IronMan.jpg");

    setCardNumber(c10, 34);
    setCardNumber(c11, 6);
    setCardNumber(c12, 12);
    setCardNumber(c13, 1);
    setCardNumber(c14, 42);
    setCardNumber(c15, 23);
    setCardNumber(c16, 66);
    setCardNumber(c17, 7);
    setCardNumber(c18, 3);
    setCardNumber(c19, 45);
    setCardNumber(c20, 99);

    createPrintable(p3);
    saveProject(p3);

    return 0;
}
