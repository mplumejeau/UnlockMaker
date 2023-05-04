/*
// Created by vboxuser on 01/05/23.
*/
#include <stdio.h>
#include <string.h>
#include <graphviz/gvc.h>

#include "../View/viewApplication.h"

#include "../Model/Project.h"
#include "../Model/VertexList.h"
#include "../Model/EdgeList.h"
#include "../Model/Card.h"
#include "../Model/Link.h"
#include "../Model/Struct.h"

#include "Controller.h"

static Project *curProject = NULL;
static Card *cardSelected = NULL;
static GtkWidget *cardSelectedBtn = NULL;
static char graphDataPath[100] = "";
static char graphPNGPath[100] = "";

int main(int argc, char* argv[]) {
#ifdef GTK_SRCDIR
    g_chdir (GTK_SRCDIR);
#endif

    int status;

    GtkApplication *app = gtk_application_new("org.unlock-maker", 0);
    g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return status;
}

void createNewProject_cb(GtkApplication *app, gpointer user_data){

    curProject = allocProject();
    /*Temporary projet path and name, to change depending on OS*/
    initProject(curProject, ".", "NewProject");
    initGraphFile(".");
}

void initGraphFile() {
    strcat(graphDataPath, curProject->path);
    strcat(graphDataPath, "/");
    strcat(graphDataPath, curProject->name);
    strcpy(graphPNGPath, graphDataPath);
    strcat(graphDataPath, "/graphData.txt");
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "w");
    if(fp == NULL) exit(0);
    fprintf(fp, "digraph G {\nbgcolor=lightgrey;\nnode [style=filled,color=grey];\n\nnode [style=filled,color=blue];\n\nnode [style=filled,color=red];\n\nnode [style=filled,color=green];\n\nedge [color=black]\n\nedge [color=red]\n\nedge [color=magenta]\n\nedge [color=orange]\n\n}");
    fclose(fp);
    strcat(graphPNGPath, "/graph.png");
    changeCenterImagePath(graphPNGPath);
    exportPNGGraphFromFile(graphDataPath, graphPNGPath);
    reloadCenterImage();
}

void updateGraphFile() {
    //To be modified
}

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

void onPressAddCard_cb(GtkWidget *widget, gpointer data) {
    Card* c = addEmptyCard(curProject);
    char btnLabel[11] = "Card n°";
    char cardId[3];
    sprintf(cardId, "%d", c->id);
    strcat(btnLabel,cardId);
    GtkWidget *button = gtk_button_new_with_label(btnLabel);
    gtk_widget_set_name(GTK_WIDGET(button), "GreyCardBtn");
    g_signal_connect (button, "clicked", G_CALLBACK(onSelectCard_cb), c);
    gtk_box_append(GTK_BOX(data), GTK_WIDGET(button));
}

void onSelectCard_cb(GtkWidget *widget, gpointer data) {
    cardSelected = data;
    cardSelectedBtn = widget;
    char panelLabel[20] = "Card Selected : ";
    char cardId[3];
    sprintf(cardId, "%d", ((Card*)data)->id);
    strcat(panelLabel,cardId);
    modifyInfoPanel(panelLabel);
}

void onPressDeleteCard_cb(GtkWidget *widget) {
    deleteCard(curProject, cardSelected);
    gtk_widget_hide(cardSelectedBtn);
    cardSelected = NULL;
    cardSelectedBtn = NULL;
    modifyInfoPanel("Aucune carte selectionnée");
}

void onPressModifyCard_cb(GtkWidget *widget, gpointer data) {

}

/* Don't work for now*/
void closeProject(GtkApplication *app, gpointer user_data){
    if(curProject != NULL){
        deleteProject(curProject);
    }
}

