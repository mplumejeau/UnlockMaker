/* Created by louenn on 14/03/23. */

#include <stdio.h>
#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <string.h>

#include "viewApplication.h"

/* This is a basic example of GUI using an XML file, it creates a window with 3 buttons */



void print_csl(GtkWidget *widget, char* data) {
    g_print(data);
    g_print("\n");
}

void quit_cb(GtkWindow *window) {
    gtk_window_close(window);
}

void changeColorPink(GtkWidget *widget) {
    if(strcmp(gtk_widget_get_name(widget),"PinkBox")==0) {
        gtk_widget_set_name(widget, "GtkBox");
    } else {
        gtk_widget_set_name(widget, "PinkBox");
    }

}

GObject* addGenericButton(GObject *button, GtkBuilder *builder, char* id) {
    button = gtk_builder_get_object(builder, id);
    if(strcmp(gtk_widget_get_name(GTK_WIDGET(button)),"GtkButton")==0) {
        gtk_widget_set_name(GTK_WIDGET(button), "genericButton");
    }
    return button;
}

void activate(GtkApplication *app, gpointer user_data) {

    GObject *window;
    GObject *button;
    load_css();
    /* Construct a GtkBuilder instance and load our UI description */
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "../View/viewBuilder.ui", NULL);

    /* Connect signal handlers to the constructed widgets. */
    window = gtk_builder_get_object(builder, "window");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 500);
    gtk_window_set_application(GTK_WINDOW (window), app);
    /*gtk_window_fullscreen(GTK_WINDOW(window));*/

    button = addGenericButton(button, builder, "buttonHello");
    g_signal_connect (button, "clicked", G_CALLBACK(print_csl), "Hello World");

    /*button = addGenericButton(button, builder, "buttonGoodbye");
    g_signal_connect (button, "clicked", G_CALLBACK(print_csl), "Goodbye World");*/

    /*button = addGenericButton(button, builder, "buttonQuit");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK(quit_cb), window);*/

    /*button = addGenericButton(button, builder, "buttonColor");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK(changeColorPink), gtk_builder_get_object(builder, "grid"));*/

    gtk_widget_show(GTK_WIDGET (window));

    /* We do not need the builder any more */
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
