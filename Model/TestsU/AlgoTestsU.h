//
// Created by maxime on 28/05/23.
//

#ifndef UNLOCKMAKER_ALGOTESTSU_H
#define UNLOCKMAKER_ALGOTESTSU_H

int setupInitProjectAlgo(void** state);
int setupInitProjectAlgoWithLoops(void** state);
int teardownInitProjectAlgo(void** state);

void testCheckLoopsWithoutLoops(void** state);
void testCheckLoopsWithLoops(void** state);

void testAssignNumbersWithEverythingOk(void** state);

#endif //UNLOCKMAKER_ALGOTESTSU_H
