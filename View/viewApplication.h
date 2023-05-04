/* Created by louenn on 14/03/23. */

#include <gtk/gtk.h>

#ifndef UNLOCKMAKER_VIEWAPPLICATION_H
#define UNLOCKMAKER_VIEWAPPLICATION_H

typedef enum {CARD, LINK} buttonType;

void quit_cb(GtkWindow *window);

void activate(GtkApplication *app, gpointer user_data);

void load_css();

GObject* addGenericButton(GObject *button, GtkBuilder *builder, char* id);

void openModifyCardWindow(gpointer data);

void entryPropertiesFromCheck(GtkWidget *checkButton, gpointer entry);

void openStartingWindow(GtkBuilder *builder, GObject *mainWindow);

void cardColorUserChange_cb(GtkWidget *widget, gpointer data);

void destroyWindow_cb(gpointer window);

void openWindow_cb(gpointer window);

void openProjectSearchWindow(GtkBuilder *builder);

void modifyInfoPanel(char* text);

void onDestroySecondWindow(GtkWidget *widget, gpointer data);

void changeCenterImagePath(char* path);

void reloadCenterImage();

#endif //UNLOCKMAKER_VIEWAPPLICATION_H
