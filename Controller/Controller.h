/*
// Created by vboxuser on 01/05/23.
*/

#ifndef UNLOCKMAKER_CONTROLLER_H
#define UNLOCKMAKER_CONTROLLER_H

/** * * Classic fonctions * * **/

void unselectCard();
void unselectLink();

void setCardFixNumber(int number); // number -1 if fixNumber disabled, the number otherwise

void retrieveParentsChildren(GtkBox *parentsBox, GtkBox *childrenBox);

void addLinkFromToggle(Card *parent, Card* child);
void deleteLinkFromToggle(Card *parent, Card* child);

/** * * Callbacks * * **/

/* Project managing callbacks */

void onConfirmNewProject(GtkWidget *saveProjectWindow);
void onConfirmOpenProject(GtkWidget *openProjectWindow);
void onSaveProject();
void onCloseNoSave();

/* Card managing callbacks */

void onAddCard();
void onSelectCard(GtkWidget *cardBtn, GtkWidget *card);
void onModifyCardType(GtkWidget *dropDown);
void onToggleSetCardAsRoot(GtkWidget *checkButton);
void onToggleFixedNumberCheck(GtkWidget *checkButton, gpointer entry);
void onEnterCardFixedNumber(GtkWidget *entry, gpointer checkButton);
void onConfirmImportCardImage(GtkWidget *importImageWindow);
void onToggleAddParent(GtkWidget *parentCheckBtn, Card *parentCard);
void onToggleAddChild(GtkWidget *childCheckBtn, Card *childCard);
void onPressDeleteCard();

/* Link managing callbacks */

void onSelectLink(GtkWidget *widget, gpointer data);
void onModifyLinkType(GtkWidget *dropDown);
void onPressDeleteLink();

/* Image managing callbacks */

void onChangeImageZoom(GtkWidget *zoomBtn, gpointer image);

#endif /*UNLOCKMAKER_CONTROLLER_H*/
