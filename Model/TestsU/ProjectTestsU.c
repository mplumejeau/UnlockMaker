//
// Created by maxime on 28/05/23.
//

#include <stdio.h>
#include <stdlib.h>

#include <setjmp.h>
#include <cmocka.h>

#include "../Project.h"
#include "../Link.h"
#include "../Card.h"
#include "../VertexList.h"
#include "../EdgeList.h"

#include "ProjectTestsU.h"

const char* projectPath = "/home/miniya/Documents";
const char* imagePath = "/home/miniya/Pictures/IronMan.jpg";

int setupInitProject(void** state){
    Project* p = allocProject();
    initProject(p, projectPath, "unlockTestsU");
    Card* c0 = addEmptyCard(p);
    Card* c1 = addEmptyCard(p);
    Card* c2 = addEmptyCard(p);
    Link* l0 = addLink(p, c0, c1, DEFAULT);
    Link* l1 = addLink(p, c0, c2, DEFAULT);
    if(p != NULL){
        *state = p;
        return 0;
    } else {
        return -1;
    }
}

int teardownInitProject(void** state){
    Project* p = (Project*) (*state);
    deleteProject(p);
    free(p);
}

void testAddEmptyCard(void** state){
    Project* p = (Project*) (*state);
    int nbCardBefore = p->nbCards;
    addEmptyCard(p);
    assert_int_equal(nbCardBefore+1, p->nbCards);
}

void testSetCardImage(void** state){
    Project* p = (Project*) (*state);
    setOnFirstVertex(&p->cardList);
    setCardImage(p, p->cardList.current->card, imagePath);
    assert_int_equal(1, p->cardList.current->card->image);
}

void testDeleteCard(void** state){
    Project* p = (Project*) (*state);
    int nbCardBefore = p->nbCards;
    setOnFirstVertex(&p->cardList);
    deleteCard(p, p->cardList.current->card);
    assert_int_equal(nbCardBefore-1, p->nbCards);
}

void testAddLink(void** state){
    Project* p = (Project*) (*state);
    int nbLinkBefore = p->nbLinks;
    setOnFirstVertex(&p->cardList);
    setOnNextVertex(&p->cardList);
    Card* c1 = p->cardList.current->card;
    setOnNextVertex(&p->cardList);
    Card* c2 = p->cardList.current->card;
    Link* l2 = addLink(p, c1, c2, DEFAULT);
    assert_int_equal(nbLinkBefore+1, p->nbLinks);
    assert_int_equal(c1->id, l2->parent->id);
    assert_int_equal(c2->id, l2->child->id);
}

void testDeleteLink(void** state){
    Project* p = (Project*) (*state);
    int nbLinkBefore = p->nbLinks;
    setOnFirstEdge(&p->linkList);
    deleteLink(p, p->linkList.current->link);
    assert_int_equal(nbLinkBefore-1, p->nbLinks);
}

void testDeleteLinkFromCards(void** state){
    Project* p = (Project*) (*state);
    int nbLinkBefore = p->nbLinks;
    setOnFirstVertex(&p->cardList);
    Card* c0 = p->cardList.current->card;
    setOnNextVertex(&p->cardList);
    Card* c1 = p->cardList.current->card;
    deleteLinkFromCards(p, c0, c1);
    assert_int_equal(nbLinkBefore-1, p->nbLinks);
}

void testSetBackImage(void** state){
    Project* p = (Project*) (*state);
    setBackImage(p, imagePath);
    assert_int_equal(1, p->backImage);
}

void testSetTopImage(void** state){
    Project* p = (Project*) (*state);
    setTopImage(p, imagePath);
    assert_int_equal(1, p->topImage);
}

void testSetBottomImage(void** state){
    Project* p = (Project*) (*state);
    setBottomImage(p, imagePath);
    assert_int_equal(1, p->bottomImage);
}
