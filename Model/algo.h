//
// Created by miniya on 16/04/23.
//

#ifndef UNLOCKMAKER_ALGO_H
#define UNLOCKMAKER_ALGO_H

#include "Struct.h"

int checkLoops(Project* p);

void runCompatibility(Project* p);

void runDiscard(Project* p);

void assignNumbers(Project* p);

int iterateLoops(Card* targetCard, Card* currentCard);

#endif //UNLOCKMAKER_ALGO_H


