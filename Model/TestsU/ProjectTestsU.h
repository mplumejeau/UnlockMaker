//
// Created by maxime on 28/05/23.
//

#ifndef UNLOCKMAKER_PROJECTTESTSU_H
#define UNLOCKMAKER_PROJECTTESTSU_H

int setupInitProject(void** state);
int teardownInitProject(void** state);

void testSetRoot(void** state);

void testAddEmptyCard(void** state);
void testSetCardImage(void** state);
void testDeleteCard(void** state);

void testAddLink(void** state);
void testDeleteLink(void** state);
void testDeleteLinkFromCards(void** state);

void testSetBackImage(void** state);
void testSetTopImage(void** state);
void testSetBottomImage(void** state);

void testSaveAndLoadProject(void** state);

#endif //UNLOCKMAKER_PROJECTTESTSU_H
