/* Created by louenn on 14/03/23. */

#include <gtk/gtk.h>
#include "../Model/Project.h"

#ifndef UNLOCKMAKER_VIEWAPPLICATION_H
#define UNLOCKMAKER_VIEWAPPLICATION_H

typedef enum {CARD, LINK} buttonType;

void activate(GtkApplication *app);

void load_css();

void addCardBtn(Card* c, GtkWidget* box);

void addCardLblForModify(Card* c, GtkWidget* checkBtn, GtkWidget* box);

void addLinkBtn(Link* l);

void removeLinkBtnFromCards(Card* parent, Card* child);

void openModifyCardWindow_cb();

void openModifyLinkWindow_cb();

void entryPropertiesFromCheck(GtkWidget *checkButton, gpointer entry);

void openStartingWindow(GtkWindow *mainWindow);
void newProjectBrowse_cb(GtkWidget* mainWindow);
void openProjectBrowse_cb(GtkWidget* mainWindow);

//A mettre dans controleur
void cardColorUserChange_cb(GtkWidget *widget);
void linkTypeUserChange_cb(GtkWidget *widget);


void destroyWindow_cb(GtkWindow *window);

void openMainWindow_cb(GtkWindow *window);

int openConfirmationWindow_cb(GtkWindow *mainWindow);

void modifyInfoPanel(char* text);

void onDestroySecondWindow_cb();

void setCenterImagePath(char* path);

void reloadCenterImage();

void disableRightCardButtons();

void disableRightLinkButtons();

void enableRightCardButtons();

void enableRightLinkButtons();

#endif //UNLOCKMAKER_VIEWAPPLICATION_H
