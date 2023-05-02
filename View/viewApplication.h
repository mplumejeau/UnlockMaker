/* Created by louenn on 14/03/23. */

#include <gtk/gtk.h>

#ifndef UNLOCKMAKER_VIEWAPPLICATION_H
#define UNLOCKMAKER_VIEWAPPLICATION_H

typedef enum {CARD, LINK} buttonType;

void print_csl(GtkWidget *widget, gpointer data);

void quit_cb(GtkWindow *window);

void activate(GtkApplication *app, gpointer user_data);

void load_css();

GObject* addGenericButton(GObject *button, GtkBuilder *builder, char* id);

void newCard(gpointer data);

void selectCard(GtkWidget *widget);

void openModifyCardWindow(gpointer data);

void entryPropertiesFromCheck(GtkWidget *checkButton, gpointer entry);

void openStartingWindow(GtkBuilder *builder, GObject *mainWindow);

void destroyWindow_cb(gpointer window);

void unminimizeWindow_cb(gpointer window);

void openProjectSearchWindow(GtkBuilder *builder);

void modifyInfoPanel(char* text);

void onDestroy(GtkWidget *widget, gpointer data);

void onDestroySecondWindow(GtkWidget *widget, gpointer data);

int exportPNGGraphFromFile(char* dataInput, char* PNGOutput);

#endif //UNLOCKMAKER_VIEWAPPLICATION_H
