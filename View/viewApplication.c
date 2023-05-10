/* Created by louenn on 14/03/23. */

#include <stdio.h>
#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <string.h>


#include "viewApplication.h"
#include "../Model/Project.h"

#include "../Controller/Controller.h"

bool secondWindowOpen = false;
GObject *infoPanel = NULL;
char imagePath[MAXPATH+16] = "";
extern Card *cardSelected;
extern Link *linkSelected;
extern Project *curProject;
GObject *centerImage = NULL;
GObject *btnModifyCard = NULL;
GObject *btnDeleteCard = NULL;
GObject *btnModifyLink = NULL;
GObject *btnDeleteLink = NULL;
GObject *linkBox = NULL;


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

void addLinkBtn(Link* l, Card* parent, Card* child) {
    char btnLabel[12] = "";
    sprintf(btnLabel, "%d -> %d", parent->id, child->id);
    GtkWidget *button = gtk_button_new_with_label(btnLabel);
    gtk_widget_set_name(GTK_WIDGET(button), "unselectedFoundLinkBtn");
    g_signal_connect (button, "clicked", G_CALLBACK(onSelectLink_cb), l);
    gtk_box_append(GTK_BOX(linkBox), GTK_WIDGET(button));
}

void openModifyCardWindow_cb() {
    if(secondWindowOpen == 0 && cardSelected != NULL) {
        GtkBuilder *builder = gtk_builder_new();
        GObject *modifWindow, *colorList, *numberCheck,*numberEntry, *rootCheck, *gridParents, *gridChildren, *closeWindowBtn;
        char windowTitle[31];
        sprintf(windowTitle, "Modification de la carte n°%d", cardSelected->id);
        gtk_builder_add_from_file(builder, "../View/viewBuilder.ui", NULL);
        modifWindow = gtk_builder_get_object(builder, "cardWindow");
        gtk_window_set_title(GTK_WINDOW(modifWindow), windowTitle);
        numberCheck = gtk_builder_get_object(builder, "numberCheck");
        numberEntry = gtk_builder_get_object(builder, "numberEntry");
        rootCheck = gtk_builder_get_object(builder, "rootCheck");
        colorList = gtk_builder_get_object(builder, "colorDropDown");
        gridParents = gtk_builder_get_object(builder, "ParentGrid");
        gridChildren = gtk_builder_get_object(builder, "ChildGrid");
        gtk_drop_down_set_selected(GTK_DROP_DOWN(colorList), cardSelected->type);
        gtk_check_button_set_active(GTK_CHECK_BUTTON(numberCheck), cardSelected->fixedNumber);
        if(cardSelected->fixedNumber == 1) {
            GObject *entryBuffer = G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(numberEntry)));
            char numberFixed[3];
            sprintf(numberFixed, "%d", cardSelected->number);
            gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(entryBuffer), numberFixed, (int)strlen(numberFixed));
        }
        gtk_check_button_set_active(GTK_CHECK_BUTTON(rootCheck), curProject->root == cardSelected);
        retrieveParentsChildren(GTK_WIDGET(gridParents), GTK_WIDGET(gridChildren));
        entryPropertiesFromCheck(GTK_WIDGET(numberCheck), numberEntry);
        g_signal_connect(GTK_WIDGET(colorList), "notify", G_CALLBACK(cardColorUserChange_cb), NULL);
        g_signal_connect(GTK_WIDGET(numberEntry), "changed", G_CALLBACK(onEnterCardFixedNumber_cb), numberCheck);
        g_signal_connect(GTK_WIDGET(numberCheck), "toggled", G_CALLBACK(entryPropertiesFromCheck), numberEntry);
        g_signal_connect(GTK_WIDGET(rootCheck), "toggled", G_CALLBACK(onToggleCardRoot_cb), NULL);
        closeWindowBtn = gtk_builder_get_object(builder, "closeCardWindowBtn");
        g_signal_connect_swapped(closeWindowBtn, "clicked", G_CALLBACK(quit_cb), modifWindow);
        gtk_window_set_default_size(GTK_WINDOW(modifWindow), 350, 700);
        gtk_window_set_resizable(GTK_WINDOW(modifWindow),false);
        g_signal_connect(modifWindow, "destroy", G_CALLBACK(onDestroySecondWindow), NULL);
        secondWindowOpen = true;
        gtk_widget_show(GTK_WIDGET (modifWindow));
        g_object_unref(builder);
    }
}

void openModifyLinkWindow_cb() {
    if(secondWindowOpen == 0 && cardSelected != NULL) {
        GtkBuilder *builder = gtk_builder_new();
        GObject *modifWindow, *typeList, *closeWindowBtn;
        char windowTitle[32];
        sprintf(windowTitle, "Modification du lien %d -> %d", linkSelected->parent->id, linkSelected->child->id);
        gtk_builder_add_from_file(builder, "../View/viewBuilder.ui", NULL);
        modifWindow = gtk_builder_get_object(builder, "linkWindow");
        gtk_window_set_title(GTK_WINDOW(modifWindow), windowTitle);
        typeList = gtk_builder_get_object(builder, "typeDropDown");
        gtk_drop_down_set_selected(GTK_DROP_DOWN(typeList), linkSelected->type);
        g_signal_connect(GTK_WIDGET(typeList), "notify", G_CALLBACK(linkTypeUserChange_cb), NULL);
        closeWindowBtn = gtk_builder_get_object(builder, "closeLinkWindowBtn");
        g_signal_connect_swapped(closeWindowBtn, "clicked", G_CALLBACK(quit_cb), modifWindow);
        gtk_window_set_default_size(GTK_WINDOW(modifWindow), 350, 100);
        gtk_window_set_resizable(GTK_WINDOW(modifWindow),false);
        g_signal_connect(modifWindow, "destroy", G_CALLBACK(onDestroySecondWindow), NULL);
        secondWindowOpen = true;
        gtk_widget_show(GTK_WIDGET (modifWindow));
        g_object_unref(builder);
    }
}

void cardColorUserChange_cb(GtkWidget *dropDown) {
    onModifyCardType_cb((int)gtk_drop_down_get_selected(GTK_DROP_DOWN(dropDown)));
}

void linkTypeUserChange_cb(GtkWidget *dropDown) {
    onModifyLinkType_cb((int)gtk_drop_down_get_selected(GTK_DROP_DOWN(dropDown)));
}

void entryPropertiesFromCheck(GtkWidget *checkButton, gpointer entry) {
    gboolean active = gtk_check_button_get_active(GTK_CHECK_BUTTON(checkButton));
    if(!active) {
        onToggleCardFixedNumber_cb(-1);
        gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(entry)))), "", 0);
    }
    gtk_widget_set_can_target(GTK_WIDGET(entry), active);
}

void openStartingWindow(GtkBuilder *builder, GObject *mainWindow) {
    GObject *startWindow, *newProjectButton;
    startWindow = gtk_builder_get_object(builder, "startWindow");
    newProjectButton = gtk_builder_get_object(builder, "newProjectButton");
    gtk_window_set_default_size(GTK_WINDOW(startWindow), 300, 200);
    //Création du projet
    g_signal_connect(newProjectButton, "clicked", G_CALLBACK(createNewProject_cb), NULL);
    g_signal_connect_swapped(newProjectButton, "clicked", G_CALLBACK(openMainWindow_cb), mainWindow);
    g_signal_connect_swapped(newProjectButton, "clicked", G_CALLBACK(destroyWindow_cb), startWindow);

    gtk_widget_show(GTK_WIDGET (startWindow));
}

void destroyWindow_cb(gpointer window) {
    gtk_window_close(GTK_WINDOW(window));
}

void openMainWindow_cb(gpointer window) {
    char windowName[MAXNAME+14] = "";
    strcpy(windowName, curProject->name);
    strcat(windowName, " - UnlockMaker");
    gtk_window_set_title(GTK_WINDOW(window), windowName);
    gtk_widget_show(GTK_WIDGET(window));
    gtk_window_maximize(GTK_WINDOW(window));
}

void openProjectSearchWindow(GtkBuilder *builder) {

}

void modifyInfoPanel(char* text) {
    gtk_label_set_label(GTK_LABEL(infoPanel), text);
}

void onDestroySecondWindow(GtkWidget *widget, gpointer data) {
    secondWindowOpen = false;
}

void setCenterImagePath(char* path) {
    strcpy(imagePath, path);
    gtk_image_set_from_file(GTK_IMAGE(centerImage), imagePath);
}

void reloadCenterImage() {
    gtk_image_set_from_file(GTK_IMAGE(centerImage), imagePath);
}

void disableRightCardButtons() {
    gtk_widget_set_can_target(GTK_WIDGET(btnModifyCard),false);
    gtk_widget_set_can_target(GTK_WIDGET(btnDeleteCard),false);
}

void disableRightLinkButtons() {
    gtk_widget_set_can_target(GTK_WIDGET(btnModifyLink),false);
    gtk_widget_set_can_target(GTK_WIDGET(btnDeleteLink),false);
}

void enableRightCardButtons() {
    gtk_widget_set_can_target(GTK_WIDGET(btnModifyCard),true);
    gtk_widget_set_can_target(GTK_WIDGET(btnDeleteCard),true);
}

void enableRightLinkButtons() {
    gtk_widget_set_can_target(GTK_WIDGET(btnModifyLink),true);
    gtk_widget_set_can_target(GTK_WIDGET(btnDeleteLink),true);
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

    button = addGenericButton(button, builder, "Quit");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK(quit_cb), window);
    g_signal_connect(window, "destroy", G_CALLBACK(closeProject), NULL);

    button = gtk_builder_get_object(builder, "BtnAddCard");
    box = gtk_builder_get_object(builder, "CardBox");
    g_signal_connect(button, "clicked", G_CALLBACK(onAddCard_cb), box);

    btnModifyCard = gtk_builder_get_object(builder, "BtnModifyCard");
    g_signal_connect_swapped (btnModifyCard, "clicked", G_CALLBACK(openModifyCardWindow_cb), builder);

    btnDeleteCard = gtk_builder_get_object(builder, "BtnDeleteCard");
    g_signal_connect(btnDeleteCard, "clicked", G_CALLBACK(onPressDeleteCard_cb), box);

    btnModifyLink = gtk_builder_get_object(builder, "BtnModifyLink");
    g_signal_connect_swapped (btnModifyLink, "clicked", G_CALLBACK(openModifyLinkWindow_cb), builder);

    btnDeleteLink = gtk_builder_get_object(builder, "BtnDeleteLink");
    g_signal_connect(btnDeleteLink, "clicked", G_CALLBACK(onPressDeleteLink_cb), box);

    disableRightCardButtons();
    disableRightLinkButtons();

    infoPanel = gtk_builder_get_object(builder, "infoPanel");
    centerImage = gtk_builder_get_object(builder, "GraphViewer");
    linkBox = gtk_builder_get_object(builder, "linkBox");

    secondWindowOpen = false;
    openStartingWindow(builder, window);

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


