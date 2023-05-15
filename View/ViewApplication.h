/* Created by louenn on 14/03/23. */

#include <gtk/gtk.h>
#include "../Model/Project.h"

#ifndef UNLOCKMAKER_VIEWAPPLICATION_H
#define UNLOCKMAKER_VIEWAPPLICATION_H

/* View initialization functions */

void initView(GtkApplication *app);
void loadCss();

/* Widget adding/removing functions */

void addCardBtn(Card* c);
void addCardLblForModify(Card* c, GtkWidget* checkBtn, GtkWidget* box);

void addLinkBtn(Link* l);
void removeLinkBtnFromCards(Card* parent, Card* child);

/* Window opening functions */

void openStartingWindow(GtkWindow *mainWindow);
void openNewProjectBrowser(GtkWidget* mainWindow);
void openOpenProjectBrowser(GtkWidget* mainWindow);

void openMainWindow(GtkWindow *window);

void openModifyCardWindow();
void openModifyLinkWindow();
void onDestroySecondWindow_cb();

void openImportCardImageBrowser();
void openCardImageViewer();

int openCloseConfirmationWindow(GtkWindow *mainWindow);

/* Widget managing functions */

void refreshCardLabel();
void refreshLinkLabel();

void changeSelectedCardLabel(char* text);
void changeSelectedLinkLabel(char* text);
void changeInfoLabel(char* text);

void setCenterImagePath(char* path);
void reloadCenterImage();

void disableRightCardButtons();
void disableRightLinkButtons();
void enableRightCardButtons();
void enableRightLinkButtons();

#endif //UNLOCKMAKER_VIEWAPPLICATION_H
