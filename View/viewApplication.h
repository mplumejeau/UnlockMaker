/* Created by louenn on 14/03/23. */

#include <gtk/gtk.h>

#ifndef UNLOCKMAKER_VIEWAPPLICATION_H
#define UNLOCKMAKER_VIEWAPPLICATION_H

typedef enum {CARD, LINK} buttonType;

void print_csl(GtkWidget *widget, gpointer data);

void quit_cb(GtkWindow *window);

void activate(GtkApplication *app, gpointer user_data);

void load_css();

void changeColorPink(GtkWidget *widget);

GObject* addGenericButton(GObject *button, GtkBuilder *builder, char* id);

GtkWidget* newCard(GtkWidget *button, GObject *box);

void openModifyCardWindow(GObject);

void onDestroy(GtkWidget *widget, gpointer data);

#endif //UNLOCKMAKER_VIEWAPPLICATION_H
