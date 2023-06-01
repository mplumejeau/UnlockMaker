//
// Created by miniya on 25/05/23.
//

#ifndef UNLOCKMAKER_CARDTESTSU_H
#define UNLOCKMAKER_CARDTESTSU_H

int setupAllocCard(void** state);

int teardownAllocCard(void** state);

void testsInitEmptyCard(void** state);

void testSetCardType(void** state);

void testSetCardNumber(void** state);

void testFixCardNumber(void** state);

void testUnfixCardNumber(void** state);

#endif //UNLOCKMAKER_CARDTESTSU_H