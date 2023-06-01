//
// Created by miniya on 25/05/23.
//

#ifndef UNLOCKMAKER_CARDTESTSU_H
#define UNLOCKMAKER_CARDTESTSU_H

#endif //UNLOCKMAKER_CARDTESTSU_H


#ifndef CARDTESTSU_H
#define CARDTESTSU_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

// Fonctions de configuration et de nettoyage des tests
int setupAllocCard(void** state);
int teardownAllocCard(void** state);

// Déclarations des fonctions de test
void testSetCardType(void** state);
void testSetCardNumber(void** state);
void testFixCardNumber(void** state);

#endif // CARDTESTSU_H

