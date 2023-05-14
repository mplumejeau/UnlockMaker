/*
// Created by vboxuser on 01/05/23.
*/

#ifndef UNLOCKMAKER_CONTROLLER_H
#define UNLOCKMAKER_CONTROLLER_H

void createNewProject_cb();
void onConfirmNewProject_cb(GtkWidget *saveProjectWindow);

void onAddCard_cb();
void onSelectCard_cb(GtkWidget *cardBtn, gpointer card);
void unselectCard();
void onSelectLink_cb(GtkWidget *widget, gpointer data);
void unselectLink();
void onPressDeleteCard_cb();
void onPressDeleteLink_cb();
void onModifyCardType_cb(int newType);
void onModifyLinkType_cb(int newType);
void onToggleCardFixedNumber_cb(int number); // -1 if fixNumber disabled, the number otherwise
void onEnterCardFixedNumber_cb(GtkWidget *entry, gpointer checkButton);
void onToggleCardRoot_cb(GtkWidget *checkButton);

void retrieveParentsChildren(GtkBox *parentsBox, GtkBox *childrenBox);

void onToggleAddParent_cb(GtkWidget *widget, gpointer data);
void onToggleAddChild_cb(GtkWidget *widget, gpointer data);

void changeImageZoom_cb(GtkWidget *zoomBtn, gpointer image);

void onConfirmOpenProject_cb(GtkWidget *openProjectWindow);
void onConfirmSaveProjectAs_cb(GtkWidget *saveProjectWindow);

void onPressSaveProject_cb();
void onPressSaveProjectAs_cb();

void onCloseNoSave_cb();

#endif /*UNLOCKMAKER_CONTROLLER_H*/
