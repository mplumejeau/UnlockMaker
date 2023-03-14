/* Created by louenn on 14/03/23. */

#include <stdio.h>
#include <gtk/gtk.h>
#include <glib/gstdio.h>

#include "viewApplication.h"

/* This is a basic example of GUI using an XML file, it creates a window with 3 buttons */

void print_hello(GtkWidget *widget, gpointer data) {
    g_print("Hello World\n");
}

void quit_cb(GtkWindow *window) {
    gtk_window_close(window);
}

void activate(GtkApplication *app, gpointer user_data) {

    GObject *window;
    GObject *button;
    /* Construct a GtkBuilder instance and load our UI description */
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "../View/viewBuilder.ui", NULL);

    /* Connect signal handlers to the constructed widgets. */
    window = gtk_builder_get_object(builder, "window");
    gtk_window_set_application(GTK_WINDOW (window), app);

    button = gtk_builder_get_object(builder, "button1");
    g_signal_connect (button, "clicked", G_CALLBACK(print_hello), NULL);

    button = gtk_builder_get_object(builder, "button2");
    g_signal_connect (button, "clicked", G_CALLBACK(print_hello), NULL);

    button = gtk_builder_get_object(builder, "quit");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK(quit_cb), window);

    gtk_widget_show(GTK_WIDGET (window));

    /* We do not need the builder any more */
    g_object_unref(builder);
}
