/* Created by louenn on 14/03/23. */

#include <stdio.h>
#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <string.h>
#include <graphviz/gvc.h>

#include "viewApplication.h"

#include "../Controller/Controller.h"

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
        GObject *modifWindow, *colorList, *numberCheck, *numberEntry;
        gtk_builder_add_from_file(builder, "../View/viewBuilder.ui", NULL);
        modifWindow = gtk_builder_get_object(builder, "modifWindow");
        numberCheck = gtk_builder_get_object(builder, "numberCheck");
        numberEntry = gtk_builder_get_object(builder, "numberEntry");
        //Récupérer les informations de la carte pour initialiser les valeurs
        entryPropertiesFromCheck(GTK_WIDGET(numberCheck), numberEntry);
        g_signal_connect(GTK_WIDGET(numberCheck), "toggled", G_CALLBACK(entryPropertiesFromCheck), numberEntry);
        //colorList = gtk_builder_get_object(builder, "colorList");
        //gtk_string_list_append(GTK_STRING_LIST(colorList), "Pink");
        gtk_window_set_default_size(GTK_WINDOW(modifWindow), 300, 700);
        gtk_window_set_resizable(GTK_WINDOW(modifWindow),false);
        g_signal_connect(modifWindow, "destroy", G_CALLBACK(onDestroySecondWindow), NULL);
        secondWindowOpen = 1;
        gtk_widget_show(GTK_WIDGET (modifWindow));
        g_object_unref(builder);
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

//projectPath unused if newProject == 1
void initGraphViz(int newProject, char* projectPath /* Or a pointer to the project */) {
    //create a text file
    //if newProject == 0 then fill it with the data from the project
    //update the graph on the view
}

//this function must be called at every modification/addition/suppression of any Card/link
void updateImageGraphViz() {
    //call exportPNGGraphFromFile to create the new PNG image
    //display it on the screen with gtk_image_set_from_file(Gtk_Image *image, char* filename);
}

/**
 * Takes a text file in the format of graphviz and exports it on a png format
 * @param graphInput The input file containing the text graph data
 * @param PNGoutput The output file containing the PNG graph image
 * @return 1 if the function was successful, 0 otherwise
 */
int exportPNGGraphFromFile(char* dataInput, char* PNGOutput) {
    Agraph_t *g;
    GVC_t *gvc;
    gvc = gvContext();
    //g = agopen("G", Agdirected, NULL);
    FILE *fp = NULL;
    fp = fopen(dataInput, "r");
    if(fp == NULL) return 0;
    g = agread(fp, NULL);
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", PNGOutput);
    gvFreeLayout(gvc,g);
    agclose(g);
    gvFreeContext(gvc);
    fclose(fp);
    return 1;
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

    g_signal_connect(window, "destroy", G_CALLBACK(closeProject), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(onDestroy), NULL);

    button = addGenericButton(button, builder, "BtnAddCard");
    box = gtk_builder_get_object(builder, "CardBox");
    g_signal_connect(button, "clicked", G_CALLBACK(addNewCard),NULL);
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
