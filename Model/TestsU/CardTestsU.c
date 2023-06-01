//
// Created by miniya on 25/05/23.
//

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
    Card* c = (Card*)*state;
    freeCard(c);
    return 0;
}

void testSetCardType(void** state) {
    Card* c = (Card*)*state;
    cardType t = GREY;
    setCardType(c, t);
    assert_int_equal(GREY, c->type);
}

void testSetCardNumber(void** state) {
    Card* c = (Card*)*state;
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

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test_setup_teardown(testSetCardType, setupAllocCard, teardownAllocCard),
            cmocka_unit_test_setup_teardown(testSetCardNumber, setupAllocCard, teardownAllocCard),
            cmocka_unit_test_setup_teardown(testFixCardNumber, setupAllocCard, teardownAllocCard),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
