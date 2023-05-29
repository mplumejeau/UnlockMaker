#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Project.h"
#include "Card.h"
#include "VertexList.h"
#include "EdgeList.h"
#include "algo.h"

int main()
{

    Project* p1 = NULL;
    Project* p2 = NULL;
    Project* p3 = NULL;

    Card* c0 = NULL;
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

    Link* l0 = NULL;
    Link* l1 = NULL;
    Link* l2 = NULL;
    Link* l3 = NULL;
    Link* l4 = NULL;
    Link* l5 = NULL;
    Link* l6 = NULL;
    Link* l7 = NULL;
    Link* l8 = NULL;
    Link* l9 = NULL;
    Link* l10 = NULL;
    Link* l11 = NULL;
    Link* l12 = NULL;
    Link* l13 = NULL;
    Link* l14 = NULL;
    Link* l15 = NULL;
    Link* l16 = NULL;
    Link* l17 = NULL;
    Link* l18 = NULL;
    Link* l19 = NULL;
    Link* l20 = NULL;
    Link* l21 = NULL;

    /*
    // demo 1 : creation, completion and save of a project

    p1 = allocProject();
    initProject(p1, "/home/maxime/Documents", "unlock4");

    setBackImage(p1, "/home/maxime/Pictures/IronMan.jpg");
    setTopImage(p1, "/home/maxime/Pictures/IronMan.jpg");
    setBottomImage(p1, "/home/maxime/Pictures/IronMan.jpg");

    c0 = addEmptyCard(p1);
    c1 = addEmptyCard(p1);
    c2 = addEmptyCard(p1);
    c3 = addEmptyCard(p1);
    c4 = addEmptyCard(p1);
    c5 = addEmptyCard(p1);

    setRoot(p1,c0);

    l0 = addLink(p1, c0, c1, DEFAULT);
    l1 = addLink(p1, c0, c2, DEFAULT);
    l2 = addLink(p1, c2, c3, DEFAULT);
    l3 = addLink(p1, c2, c4, DEFAULT);
    l4 = addLink(p1, c1, c5, COMBINE);
    l5 = addLink(p1, c3, c5, COMBINE);

    setCardImage(p1,c0, "/home/maxime/Pictures/IronMan.jpg");
    setCardImage(p1,c1, "/home/maxime/Pictures/IronMan.jpg");
    setCardImage(p1,c3, "/home/maxime/Pictures/IronMan.jpg");
    setCardImage(p1,c5, "/home/maxime/Pictures/IronMan.jpg");

    setCardNumber(c1, 15);
    setCardNumber(c2, 33);
    setCardNumber(c4, 6);

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

    /*
    // demo 3 : creation of several cards with images and creation of a pdf file

    p3 = allocProject();
    initProject(p3, "/home/maxime/Documents", "unlock3");

    setBackImage(p3, "/home/maxime/Pictures/DosCarte.jpg");
    setTopImage(p3, "/home/maxime/Pictures/BanniereChateau.jpg");
    setBottomImage(p3, "/home/maxime/Pictures/Parchemin.jpg");

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
    setCardImage(p3,c18, "/home/maxime/Pictures/Epee.jpg");
    setCardImage(p3,c19, "/home/maxime/Pictures/Chateau.jpg");
    setCardImage(p3,c20, "/home/maxime/Pictures/Foret.jpg");

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
    */

    /*
    // demo 4 : test de checkLoops et assignNumbers

    p3 = allocProject();
    initProject(p3, "/home/miniya/Documents", "unlock3");

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

    setRoot(p3, c11);

    l1 = addLink(p3, c11, c12, DEFAULT);
    l2 = addLink(p3, c12, c15, DEFAULT);
    l3 = addLink(p3, c15, c11, DEFAULT);

    int res = assignNumbers(p3);
    printf("%d\n", c11->number);
    printf("%d\n", c12->number);
    printf("%d\n", c15->number);
    */

}