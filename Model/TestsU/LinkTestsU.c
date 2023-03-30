//
// Created by maxime on 26/03/23.
//

#include <stdio.h>
#include <stdlib.h>

#include <setjmp.h>
#include <cmocka.h>

#include "../Link.h"
#include "../Card.h"

#include "LinkTestsU.h"

int setupAllocLink(void** state){
    Link* l = allocLink();
    if(l != NULL){
        *state = l;
        return 0;
    } else {
        return -1;
    }
}

int teardownAllocLink(void** state){
    Link* l = (Link*) *state;
    freeLink(l);
    return 0;
}

void testSetLinkType(void** state){
    Link* l = (Link*) *state;
    linkType t = DEFAULT;
    setLinkType(l,t);
    assert_int_equal(DEFAULT,l->type);
}

void testSetParent(void** state){
    Link* l = (Link*) *state;
    Card* c = allocCard();
    setParent(l,c);
    assert_ptr_equal(c,l->parent);
}

void testSetChild(void** state){
    Link* l = (Link*) *state;
    Card* c = allocCard();
    setChild(l,c);
    assert_ptr_equal(c,l->child);
}
