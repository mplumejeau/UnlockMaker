/*
// Created by vboxuser on 01/05/23.
*/
#include <stdio.h>
#include <string.h>
#include <graphviz/gvc.h>

#include "../View/viewApplication.h"
#include "../View/GraphHandler.h"

#include "../Model/Project.h"
#include "../Model/VertexList.h"
#include "../Model/EdgeList.h"
#include "../Model/Card.h"
#include "../Model/Link.h"
#include "../Model/Struct.h"

#include "Controller.h"

Project *curProject = NULL;
Card *cardSelected = NULL;
GtkWidget *cardSelectedBtn = NULL;
extern bool secondWindowOpen;
char graphDataPath[100] = "";
char graphPNGPath[100] = "";

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
    initGraphFile();
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
    fprintf(fp, "digraph G{bgcolor=lightgrey;node[style=filled;shape=circle];\n}");
    fclose(fp);
    strcat(graphPNGPath, "/graph.png");
    exportPNGGraphFromFile(graphDataPath, graphPNGPath);
    setCenterImagePath(graphPNGPath);
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

void onAddCard_cb(GtkWidget *widget, gpointer data) {
    if(secondWindowOpen==false) {
        Card* c = addEmptyCard(curProject);
        char btnLabel[12] = "Carte n°";
        char cardId[3];
        sprintf(cardId, "%d", c->id);
        strcat(btnLabel,cardId);
        GtkWidget *button = gtk_button_new_with_label(btnLabel);
        gtk_widget_set_name(GTK_WIDGET(button), "UnselectedGreyCardBtn");
        g_signal_connect (button, "clicked", G_CALLBACK(onSelectCard_cb), c);
        gtk_box_append(GTK_BOX(data), GTK_WIDGET(button));
        addCardGraphData(cardId, GREY);
        exportPNGGraphFromFile(graphDataPath, graphPNGPath);
        reloadCenterImage();
    }
}

void onSelectCard_cb(GtkWidget *widget, gpointer data) {
    if(secondWindowOpen==false) {
        char unselectedCardNames[4][23] = {"UnselectedGreyCardBtn", "UnselectedBlueCardBtn", "UnselectedRedCardBtn", "UnselectedGreenCardBtn"};
        char selectedCardNames[4][21] = {"SelectedGreyCardBtn", "SelectedBlueCardBtn", "SelectedRedCardBtn", "SelectedGreenCardBtn"};
        if(cardSelected != NULL) {
            gtk_widget_set_name(cardSelectedBtn, unselectedCardNames[cardSelected->type]);
        }
        cardSelected = data;
        cardSelectedBtn = widget;
        gtk_widget_set_name(cardSelectedBtn, selectedCardNames[cardSelected->type]);
        char panelLabel[26] = "Carte sélectionnée : ";
        char cardId[3];
        sprintf(cardId, "%d", ((Card*)data)->id);
        strcat(panelLabel,cardId);
        modifyInfoPanel(panelLabel);
        enableRightCardButtons();
    }

}

void onPressDeleteCard_cb(GtkWidget *widget) {
    if(cardSelected!=NULL && secondWindowOpen==false) {
        char cardId[3];
        sprintf(cardId, "%d", cardSelected->id);
        removeCardGraphData(cardId);
        exportPNGGraphFromFile(graphDataPath, graphPNGPath);
        reloadCenterImage();
        deleteCard(curProject, cardSelected);
        gtk_widget_hide(cardSelectedBtn);
        cardSelected = NULL;
        cardSelectedBtn = NULL;
        modifyInfoPanel("Aucune carte selectionnée");
        disableRightCardButtons();
    }
}

void onModifyCardType_cb(int newType) {
    if(cardSelected != NULL) {
        char cardId[3];
        sprintf(cardId, "%d", cardSelected->id);
        modifyCardTypeGraphData(cardId, newType);
        exportPNGGraphFromFile(graphDataPath, graphPNGPath);
        reloadCenterImage();
        setCardType(cardSelected, newType);
        char cardNames[4][23] = {"SelectedGreyCardBtn", "SelectedBlueCardBtn", "SelectedRedCardBtn", "SelectedGreenCardBtn"};
        gtk_widget_set_name(cardSelectedBtn, cardNames[newType]);
    }
}

void onToggleCardFixedNumber_cb(int number) {
    if(number == -1) {
        unfixCardNumber(cardSelected);
    } else {
        fixCardNumber(cardSelected, (char)number);
    }
}

void onEnterCardFixedNumber_cb(GtkWidget *entry, gpointer checkButton) {
    if(gtk_check_button_get_active(GTK_CHECK_BUTTON(checkButton))) {
        onToggleCardFixedNumber_cb(atoi(gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(entry)))))));
    }
}

void onToggleCardRoot_cb(GtkWidget *checkButton) {
    char cardId[3]="";
    sprintf(cardId, "%d", cardSelected->id);
    if(gtk_check_button_get_active(GTK_CHECK_BUTTON(checkButton))) {
        setCardAsRootGraphData(cardId);
        exportPNGGraphFromFile(graphDataPath, graphPNGPath);
        reloadCenterImage();
        setRoot(curProject, cardSelected);
    } else {
        removeCardAsRootGraphData();
        exportPNGGraphFromFile(graphDataPath, graphPNGPath);
        reloadCenterImage();
        setRoot(curProject, NULL);
    }


}

void retrieveParentsChildren(GtkWidget *gridParents, GtkWidget *gridChildren) {
    setOnFirstVertex(&(curProject->cardList));
    Card* c;
    GtkWidget *labelParent, *labelChild, *checkBtnParent, *checkBtnChild;
    char btnLabel[12], cardId[3];
    int i=0;
    while(!isOutOfListVertex(&(curProject->cardList))) {
        c=curProject->cardList.current->card;
        if(c!=cardSelected) {
            strcpy(btnLabel, "Carte n°");
            sprintf(cardId, "%d", c->id);
            strcat(btnLabel,cardId);
            labelParent = gtk_label_new(btnLabel);
            gtk_widget_set_name(GTK_WIDGET(labelParent), "ParentLabel");
            labelChild = gtk_label_new(btnLabel);
            gtk_widget_set_name(GTK_WIDGET(labelChild), "ChildLabel");
            gtk_grid_attach(GTK_GRID(gridParents), labelParent, 0, i, 8, 1);
            gtk_grid_attach(GTK_GRID(gridChildren), labelChild, 0, i, 8, 1);
            checkBtnParent = gtk_check_button_new();
            gtk_grid_attach(GTK_GRID(gridParents), checkBtnParent, 7, i, 1, 1);
            g_signal_connect(checkBtnParent, "toggled", G_CALLBACK(onToggleAddParent_cb), c);
            checkBtnChild = gtk_check_button_new();
            gtk_grid_attach(GTK_GRID(gridChildren), checkBtnChild, 7, i, 1, 1);
            g_signal_connect(checkBtnChild, "toggled", G_CALLBACK(onToggleAddChild_cb), c);
            //vérification de l'existence des liens
            setOnFirstEdge(&(cardSelected->parents));
            while(!isOutOfListEdge(&(cardSelected->parents))) {
                if (cardSelected->parents.current->link->parent == c) {
                    gtk_check_button_set_active(GTK_CHECK_BUTTON(checkBtnParent), true);
                }
                setOnNextEdge(&(cardSelected->parents));
            }
            setOnFirstEdge(&(cardSelected->children));
            while(!isOutOfListEdge(&(cardSelected->children))) {
                if (cardSelected->children.current->link->child == c) {
                    gtk_check_button_set_active(GTK_CHECK_BUTTON(checkBtnChild), true);
                }
                setOnNextEdge(&(cardSelected->children));
            }
        }
        setOnNextVertex(&(curProject->cardList));
        i++;
    }
}

void onToggleAddParent_cb(GtkWidget *checkBtn, gpointer parent) {
    gboolean active = gtk_check_button_get_active(GTK_CHECK_BUTTON(checkBtn));
    Link* l;
    char parentId[3], childId[3];
    sprintf(parentId, "%d", ((Card*)parent)->id);
    sprintf(childId, "%d", cardSelected->id);
    if(active){
        //l=addParent(cardSelected,(Card*)parent); La fonction n'est pas prête
        addLinkGraphData(parentId, childId, 0);
        exportPNGGraphFromFile(graphDataPath, graphPNGPath);
        reloadCenterImage();
    } else {
        //supprimer le lien de façon propre
        removeLinkGraphData(parentId, childId); // temporaire
        exportPNGGraphFromFile(graphDataPath, graphPNGPath);
        reloadCenterImage();
    }
    ///addParent renvoie un Link* -> l'ajouter à la liste
}

void onToggleAddChild_cb(GtkWidget *checkBtn, gpointer child) {
    gboolean active = gtk_check_button_get_active(GTK_CHECK_BUTTON(checkBtn));
    Link *l;
    char parentId[3], childId[3];
    sprintf(parentId, "%d", cardSelected->id);
    sprintf(childId, "%d", ((Card*)child)->id);
    if(active){
        // l=addChild(cardSelected, (Card*)child); La fonction n'est pas prête
        addLinkGraphData(parentId, childId, 0);
        exportPNGGraphFromFile(graphDataPath, graphPNGPath);
        reloadCenterImage();
    } else {
        //supprimer le lien de façon propre
        removeLinkGraphData(parentId, childId); // temporaire
        exportPNGGraphFromFile(graphDataPath, graphPNGPath);
        reloadCenterImage();
    }
    ///addChild renvoie un Link* -> l'ajouter à la liste
}

/* Doesn't work for now*/
void closeProject(GtkApplication *app, gpointer user_data){
    if(curProject != NULL){
        deleteProject(curProject);
    }
}

