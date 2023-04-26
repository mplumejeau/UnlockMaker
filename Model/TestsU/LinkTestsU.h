//
// Created by maxime on 26/03/23.
//

#ifndef UNLOCKMAKER_LINKTESTSU_H
#define UNLOCKMAKER_LINKTESTSU_H

int setupAllocLink(void** state);

int teardownAllocLink(void** state);

void testSetLinkType(void** state);

void testSetParent(void** state);

void testSetChild(void** state);

#endif //UNLOCKMAKER_LINKTESTSU_H
