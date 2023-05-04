/* Created by louenn on 14/03/23. */

#include <stdio.h>
#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <string.h>


#include "viewApplication.h"

#include "../Controller/Controller.h"

static int secondWindowOpen = 0;
static GObject *infoPanel = NULL;
static bool dropDownAvoidDoubles = false;
static char *imagePath = NULL;

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

void openModifyCardWindow(gpointer data) {
    if(secondWindowOpen == 0) {
        GtkBuilder *builder = gtk_builder_new();
        GObject *modifWindow, *colorList, *numberCheck, *numberEntry;
        gtk_builder_add_from_file(builder, "../View/viewBuilder.ui", NULL);
        modifWindow = gtk_builder_get_object(builder, "modifWindow");
        numberCheck = gtk_builder_get_object(builder, "numberCheck");
        numberEntry = gtk_builder_get_object(builder, "numberEntry");
        //Récupérer les informations de la carte pour initialiser les valeurs
        entryPropertiesFromCheck(GTK_WIDGET(numberCheck), numberEntry);
        g_signal_connect(GTK_WIDGET(numberCheck), "toggled", G_CALLBACK(entryPropertiesFromCheck), numberEntry);
        colorList = gtk_builder_get_object(builder, "dropDownColor");
        g_signal_connect(GTK_WIDGET(colorList), "notify", G_CALLBACK(cardColorUserChange_cb), NULL);
        gtk_drop_down_get_selected(GTK_DROP_DOWN(colorList));

        //gtk_string_list_append(GTK_STRING_LIST(colorList), "Pink");
        gtk_window_set_default_size(GTK_WINDOW(modifWindow), 300, 700);
        gtk_window_set_resizable(GTK_WINDOW(modifWindow),false);
        g_signal_connect(modifWindow, "destroy", G_CALLBACK(onDestroySecondWindow), NULL);
        secondWindowOpen = 1;
        gtk_widget_show(GTK_WIDGET (modifWindow));
        g_object_unref(builder);
    }
}

void cardColorUserChange_cb(GtkWidget *widget, gpointer data) {
    if(dropDownAvoidDoubles == false) {
        dropDownAvoidDoubles = true;
    } else {
        guint colorSelected = gtk_drop_down_get_selected(GTK_DROP_DOWN(widget));
        //envoyer notification de changement de couleur au modèle. Color id is the same as in Struct.h : 0 = Gris ; 1 = Bleu ; 2 = Rouge ; 3 = Vert
        dropDownAvoidDoubles = false;
    }
}

void entryPropertiesFromCheck(GtkWidget *checkButton, gpointer entry) {
    gboolean active = gtk_check_button_get_active(GTK_CHECK_BUTTON(checkButton));
    gtk_widget_set_can_target(GTK_WIDGET(entry), active);
}

void openStartingWindow(GtkBuilder *builder, GObject *mainWindow) {
    GObject *startWindow, *newProjectButton;
    startWindow = gtk_builder_get_object(builder, "startWindow");
    newProjectButton = gtk_builder_get_object(builder, "newProjectButton");
    gtk_window_set_default_size(GTK_WINDOW(startWindow), 300, 200);
    //Création du projet
    g_signal_connect(newProjectButton, "clicked", G_CALLBACK(createNewProject_cb), NULL);
    g_signal_connect_swapped(newProjectButton, "clicked", G_CALLBACK(openWindow_cb), mainWindow);
    g_signal_connect_swapped(newProjectButton, "clicked", G_CALLBACK(destroyWindow_cb), startWindow);

    gtk_widget_show(GTK_WIDGET (startWindow));
}

void destroyWindow_cb(gpointer window) {
    gtk_window_close(GTK_WINDOW(window));
}

void openWindow_cb(gpointer window) {
    gtk_widget_show(GTK_WIDGET(window));
    gtk_window_maximize(GTK_WINDOW(window));
}

void openProjectSearchWindow(GtkBuilder *builder) {

}

void modifyInfoPanel(char* text) {
    gtk_label_set_label(GTK_LABEL(infoPanel), text);
}

void onDestroySecondWindow(GtkWidget *widget, gpointer data) {
    secondWindowOpen = 0;
}

void changeCenterImagePath(char* path) {
    imagePath = path;
    GtkBuilder *builder = gtk_builder_new();
    GObject *image;
    gtk_builder_add_from_file(builder, "../View/viewBuilder.ui", NULL);
    image = gtk_builder_get_object(builder, "GraphViewer");
    gtk_image_set_from_file(GTK_IMAGE(image), imagePath);
}

void reloadCenterImage() {
    GtkBuilder *builder = gtk_builder_new();
    GObject *image;
    gtk_builder_add_from_file(builder, "../View/viewBuilder.ui", NULL);
    image = gtk_builder_get_object(builder, "GraphViewer");
    gtk_image_clear(GTK_IMAGE(image));
    gtk_image_set_from_file(GTK_IMAGE(image), imagePath);
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

    button = addGenericButton(button, builder, "BtnModifyCard");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK(openModifyCardWindow), builder);

    //gtk_widget_set_can_target(GTK_WIDGET(button),false);
    //gtk_widget_set_name(GTK_WIDGET(button),"RightButtonDisabled");

    button = addGenericButton(button, builder, "Quit");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK(quit_cb), window);

    g_signal_connect(window, "destroy", G_CALLBACK(closeProject), NULL);

    button = gtk_builder_get_object(builder, "BtnAddCard");
    box = gtk_builder_get_object(builder, "CardBox");
    g_signal_connect(button, "clicked", G_CALLBACK(onPressAddCard_cb), box);

    button = gtk_builder_get_object(builder, "BtnModifyCard");
    g_signal_connect(button, "clicked", G_CALLBACK(onPressModifyCard_cb), box);

    button = gtk_builder_get_object(builder, "BtnDeleteCard");
    g_signal_connect(button, "clicked", G_CALLBACK(onPressDeleteCard_cb), box);

    infoPanel = gtk_builder_get_object(builder, "infoPanel");

    secondWindowOpen = 0;
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
