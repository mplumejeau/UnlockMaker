//
// Created by miniya on 25/05/23.
//

#include <stdio.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../Card.h"

#include "CardTestsU.h"

int setupAllocCard(void** state) {
    Card* c = allocCard();
    if (c != NULL) {
        *state = c;
        return 0;
    } else {
        return -1;
    }
}

int teardownAllocCard(void** state) {
    Card* c = (Card*) *state;
    freeCard(c);
    return 0;
}

void testsInitEmptyCard(void** state){
    Card* c = (Card*) *state;
    initEmptyCard(c);
    assert_int_equal(-1, c->id);
}

void testSetCardType(void** state) {
    Card* c = (Card*) *state;
    cardType t = GREY;
    setCardType(c, t);
    assert_int_equal(GREY, c->type);
}

void testSetCardNumber(void** state) {
    Card* c = (Card*) *state;
    int n = 42;
    setCardNumber(c, n);
    assert_int_equal(42, c->number);
}

void testFixCardNumber(void** state) {
    Card* c = (Card*)*state;
    int n = 42;
    fixCardNumber(c, n);
    assert_int_equal(42, c->number);
    assert_int_equal(1, c->fixedNumber);
}

void testUnfixCardNumber(void** state){
    Card* c = (Card*)*state;
    int n = 42;
    fixCardNumber(c, n);
    unfixCardNumber(c);
    assert_int_equal(0, c->fixedNumber);
}
