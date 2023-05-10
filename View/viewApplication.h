/* Created by louenn on 14/03/23. */

#include <gtk/gtk.h>*
#include "../Model/Project.h"

#ifndef UNLOCKMAKER_VIEWAPPLICATION_H
#define UNLOCKMAKER_VIEWAPPLICATION_H

typedef enum {CARD, LINK} buttonType;

void quit_cb(GtkWindow *window);

void activate(GtkApplication *app, gpointer user_data);

void load_css();

GObject* addGenericButton(GObject *button, GtkBuilder *builder, char* id);

void addLinkBtn(Link* l, Card* parent, Card* child);

void openModifyCardWindow_cb();

void openModifyLinkWindow_cb();

void entryPropertiesFromCheck(GtkWidget *checkButton, gpointer entry);

void openStartingWindow(GtkBuilder *builder, GObject *mainWindow);

//A mettre dans controleur
void cardColorUserChange_cb(GtkWidget *widget);
void linkTypeUserChange_cb(GtkWidget *widget);


void destroyWindow_cb(gpointer window);

void openMainWindow_cb(gpointer window);

void openProjectSearchWindow(GtkBuilder *builder);

void modifyInfoPanel(char* text);

void onDestroySecondWindow(GtkWidget *widget, gpointer data);

void setCenterImagePath(char* path);

void reloadCenterImage();

void disableRightCardButtons();

void disableRightLinkButtons();

void enableRightCardButtons();

void enableRightLinkButtons();

#endif //UNLOCKMAKER_VIEWAPPLICATION_H
