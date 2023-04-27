/* Created by louenn on 14/03/23. */

#include <stdio.h>
#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <string.h>

#include "viewApplication.h"

static int secondWindowOpen = 0;
static GtkWidget *cardSelected = NULL;
static GObject *infoPanel = NULL;

void print_csl(GtkWidget *widget, gpointer data) {
    g_print(data);
    g_print("\n");
}

void quit_cb(GtkWindow *window) {
    gtk_window_close(window);
}

GObject* addGenericButton(GObject *button, GtkBuilder *builder, char* id) {
    button = gtk_builder_get_object(builder, id);
    if(strcmp(gtk_widget_get_name(GTK_WIDGET(button)),"GtkButton")==0) {
        gtk_widget_set_name(GTK_WIDGET(button), "genericButton");
    }
    return button;
}

void newCard(gpointer data) {
    GtkWidget *button = gtk_button_new_with_label("Card X");
    gtk_widget_set_name(GTK_WIDGET(button), "GreyCardBtn");
    g_signal_connect (button, "clicked", G_CALLBACK(selectCard), NULL);
    gtk_box_append(GTK_BOX(data), GTK_WIDGET(button));
}

void selectCard(GtkWidget *widget) {
    cardSelected = widget;
    modifyInfoPanel("Card Selected");
}

/*void destroyCard(GtkWidget *widget) {
    if(cardSelected == widget) {
        unselectCard();
    }
}*/

void openModifyCardWindow(gpointer data) {
    if(secondWindowOpen == 0) {
        GtkBuilder *builder = gtk_builder_new();
        gtk_builder_add_from_file(builder, "../View/viewBuilder.ui", NULL);
        GObject *modifWindow;
        modifWindow = gtk_builder_get_object(builder, "modifWindow");
        gtk_window_set_default_size(GTK_WINDOW(modifWindow), 700, 400);
        g_signal_connect(modifWindow, "destroy", G_CALLBACK(onDestroySecondWindow), NULL);
        secondWindowOpen = 1;
        gtk_widget_show(GTK_WIDGET (modifWindow));
        g_object_unref(builder);
    }
}

void openStartingWindow(GtkBuilder *builder, GObject *mainWindow) {
    GObject *startWindow, *newProjectButton;
    startWindow = gtk_builder_get_object(builder, "startWindow");
    newProjectButton = gtk_builder_get_object(builder, "newProjectButton");
    gtk_window_set_default_size(GTK_WINDOW(startWindow), 300, 200);
    g_signal_connect_swapped(newProjectButton, "clicked", G_CALLBACK(unminimizeWindow_cb), mainWindow);
    g_signal_connect_swapped(newProjectButton, "clicked", G_CALLBACK(destroyWindow_cb), startWindow);
    //Création du projet
    gtk_widget_show(GTK_WIDGET (startWindow));
}

void destroyWindow_cb(gpointer window) {
    gtk_window_close(GTK_WINDOW(window));
}

void unminimizeWindow_cb(gpointer window) {
    gtk_window_present(GTK_WINDOW(window));
    gtk_window_maximize(GTK_WINDOW(window));
}

void openProjectSearchWindow(GtkBuilder *builder) {

}

void modifyInfoPanel(char* text) {
    gtk_label_set_label(GTK_LABEL(infoPanel), text);
}

void onDestroy(GtkWidget *widget, gpointer data) {
    print_csl(widget, "Window Destroyed");
}

void onDestroySecondWindow(GtkWidget *widget, gpointer data) {
    secondWindowOpen = 0;
}

void activate(GtkApplication *app, gpointer user_data) {

    GObject *window;
    GObject *button;
    GObject *box;
    load_css();
    /* Construct a GtkBuilder instance and load our UI description */
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "../View/viewBuilder.ui", NULL);

    /* Connect signal handlers to the constructed widgets. */
    window = gtk_builder_get_object(builder, "mainWindow");
    gtk_window_set_default_size(GTK_WINDOW(window), 1700, 900);
    gtk_window_set_application(GTK_WINDOW(window), app);

    button = addGenericButton(button, builder, "buttonHello");
    g_signal_connect (button, "clicked", G_CALLBACK(print_csl), "Hello World");
    /*g_signal_connect (button, "clicked", G_CALLBACK(print_csl), windowModify);*/

    button = addGenericButton(button, builder, "BtnModifyCard");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK(openModifyCardWindow), builder);

    //gtk_widget_set_can_target(GTK_WIDGET(button),false);
    //gtk_widget_set_name(GTK_WIDGET(button),"RightButtonDisabled");

    button = addGenericButton(button, builder, "Quit");
    g_signal_connect (button, "clicked", G_CALLBACK(print_csl), "Goodbye World");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK(quit_cb), window);

    /*button = addGenericButton(button, builder, "buttonColor");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK(changeColorPink), gtk_builder_get_object(builder, "grid"));*/

    g_signal_connect(window, "destroy", G_CALLBACK(onDestroy), NULL);

    button = addGenericButton(button, builder, "BtnAddCard");
    box = gtk_builder_get_object(builder, "CardBox");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(newCard), box);

    infoPanel = gtk_builder_get_object(builder, "infoPanel");

    gtk_window_minimize(GTK_WINDOW(window));
    secondWindowOpen = 0;
    gtk_widget_show(GTK_WIDGET (window));
    openStartingWindow(builder, window);

    /* We do not need the builder anymore */
    g_object_unref(builder);
}

void load_css ( void )
{
    GtkCssProvider *CSSprovider;
    /// ***
    const gchar *css_style_file = "../View/viewStyle.css";
    GFile *css_fp               = g_file_new_for_path ( css_style_file );
    /// ***
    CSSprovider = gtk_css_provider_new ();
    /// ***
    gtk_css_provider_load_from_file( CSSprovider, css_fp);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(CSSprovider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    /// ***
    g_object_unref ( CSSprovider );
}
