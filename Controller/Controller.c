/*
// Created by vboxuser on 01/05/23.
*/
#include <stdio.h>
#include <string.h>
#include <graphviz/gvc.h>
#include <sys/stat.h>
#include <errno.h>

#include "../View/viewApplication.h"
#include "../View/GraphHandler.h"

#include "../Model/Project.h"
#include "../Model/VertexList.h"
#include "../Model/EdgeList.h"
#include "../Model/Card.h"
#include "../Model/Link.h"

#include "Controller.h"

Project *curProject = NULL;
Card *cardSelected = NULL;
Link *linkSelected = NULL;
GtkWidget *cardSelectedBtn = NULL;
GtkWidget *linkSelectedBtn = NULL;
extern bool secondWindowOpen;
char graphDirPath[MAXPATH+6] = "";
char graphDataPath[MAXPATH+20] = "";

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
    char projectPath[MAXPATH];
    strcpy(projectPath, "/home/");
    strcat(projectPath, getlogin());
    strcat(projectPath, "/Documents");
    initProject(curProject, projectPath, "Sans Titre");
    initGraphFile();
}

void initGraphFile() {
    strcat(graphDirPath, curProject->path);
    strcat(graphDirPath, "/");
    strcat(graphDirPath, curProject->name);
    strcat(graphDirPath, "/Graph");
    char graphPNGPath[MAXPATH+16];
    strcpy(graphDataPath, graphDirPath);
    strcat(graphDataPath, "/graphData.txt");
    strcpy(graphPNGPath, graphDirPath);
    strcat(graphPNGPath, "/graph.png");
    if (mkdir(graphDirPath, 0755) != 0 ) {
        fprintf( stderr, "error : creation of the directory %s is impossible\n", graphDirPath);
        fprintf(stderr,"%d\n", errno);
    }
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "w");
    if(fp == NULL) {
        fprintf(stderr, "error : creation of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
    }
    fprintf(fp, "digraph G{bgcolor=lightgrey;node[style=filled;shape=circle];\n}");
    fclose(fp);
    exportPNGGraphFromFile(graphDataPath, graphPNGPath);
    setCenterImagePath(graphPNGPath);
}


int refreshGraphPNG() {
    char graphPNGPath[MAXPATH+10];
    strcpy(graphPNGPath, graphDirPath);
    strcat(graphPNGPath, "/graph.png");
    exportPNGGraphFromFile(graphDataPath, graphPNGPath);
    reloadCenterImage();
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
        addCardGraphData(graphDataPath, cardId, GREY);
        refreshGraphPNG();
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

void onSelectLink_cb(GtkWidget *linkBtn, gpointer link) {
    if(secondWindowOpen==false) {
        char unselectedLinkNames[4][23] = {"unselectedFoundLinkBtn", "unselectedCombLinkBtn", "unselectedFixedLinkBtn", "unselectedHintLinkBtn"};
        char selectedLinkNames[4][21] = {"selectedFoundLinkBtn", "selectedCombLinkBtn", "selectedFixedLinkBtn", "selectedHintLinkBtn"};
        if(linkSelected != NULL) {
            gtk_widget_set_name(linkSelectedBtn, unselectedLinkNames[linkSelected->type]);
        }
        linkSelected = (Link*)link;
        linkSelectedBtn = linkBtn;
        gtk_widget_set_name(linkSelectedBtn, selectedLinkNames[linkSelected->type]);
        /*char panelLabel[26] = "Lien sélectionné : ";
        char cardId[3];
        sprintf(cardId, "%d", ((Card*)data)->id);
        strcat(panelLabel,cardId);
        modifyInfoPanel(panelLabel);*/
        enableRightLinkButtons();
    }
}

void onPressDeleteCard_cb(GtkWidget *widget) {
    if(cardSelected!=NULL && secondWindowOpen==false) {
        char cardId[3];
        sprintf(cardId, "%d", cardSelected->id);
        removeCardGraphData(graphDataPath, cardId);
        refreshGraphPNG();
        deleteCard(curProject, cardSelected);
        gtk_widget_hide(cardSelectedBtn);
        cardSelected = NULL;
        cardSelectedBtn = NULL;
        modifyInfoPanel("Aucune carte selectionnée");
        disableRightCardButtons();
    }
}

void onPressDeleteLink_cb(GtkWidget *widget) {
    if(linkSelected!=NULL && secondWindowOpen==false) {
        char parentId[3], childId[3];
        sprintf(parentId, "%d", linkSelected->parent->id);
        sprintf(childId, "%d", linkSelected->child->id);
        removeLinkGraphData(graphDataPath, parentId, childId);
        refreshGraphPNG();
        deleteLink(curProject, linkSelected);
        gtk_widget_hide(linkSelectedBtn);
        linkSelected = NULL;
        linkSelectedBtn = NULL;
        //modifyInfoPanel("Aucune carte selectionnée");
        disableRightLinkButtons();
    }
}

void onModifyCardType_cb(int newType) {
    if(cardSelected != NULL) {
        char cardId[3];
        sprintf(cardId, "%d", cardSelected->id);
        modifyCardTypeGraphData(graphDataPath, cardId, newType);
        refreshGraphPNG();
        setCardType(cardSelected, newType);
        char cardNames[4][23] = {"SelectedGreyCardBtn", "SelectedBlueCardBtn", "SelectedRedCardBtn", "SelectedGreenCardBtn"};
        gtk_widget_set_name(cardSelectedBtn, cardNames[newType]);
    }
}

void onModifyLinkType_cb(int newType) {
    if(linkSelected != NULL) {
        char parentId[3], childId[3];
        sprintf(parentId, "%d", linkSelected->parent->id);
        sprintf(childId, "%d", linkSelected->child->id);
        modifyLinkTypeGraphData(graphDataPath, parentId, childId, newType);
        refreshGraphPNG();
        setLinkType(linkSelected, newType);
        char linkNames[4][23] = {"selectedFoundLinkBtn", "selectedCombLinkBtn", "selectedFixedLinkBtn", "selectedHintLinkBtn"};
        gtk_widget_set_name(linkSelectedBtn, linkNames[newType]);
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
        setCardAsRootGraphData(graphDataPath, cardId);
        setRoot(curProject, cardSelected);
    } else {
        removeCardAsRootGraphData(graphDataPath);
        setRoot(curProject, NULL);
    }
    refreshGraphPNG();

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
            checkBtnChild = gtk_check_button_new();
            gtk_grid_attach(GTK_GRID(gridChildren), checkBtnChild, 7, i, 1, 1);
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
            g_signal_connect(checkBtnParent, "toggled", G_CALLBACK(onToggleAddParent_cb), c);
            g_signal_connect(checkBtnChild, "toggled", G_CALLBACK(onToggleAddChild_cb), c);
        }
        setOnNextVertex(&(curProject->cardList));
        i++;
    }
}

void onToggleAddParent_cb(GtkWidget *checkBtn, gpointer parent) {
    gboolean active = gtk_check_button_get_active(GTK_CHECK_BUTTON(checkBtn));
    char parentId[3], childId[3];
    sprintf(parentId, "%d", ((Card*)parent)->id);
    sprintf(childId, "%d", cardSelected->id);
    if(active){
        addLinkGraphData(graphDataPath, parentId, childId, DEFAULT);
        addLinkBtn(addLink(curProject, (Card*)parent, cardSelected, DEFAULT), (Card*)parent, cardSelected);
    } else {
        //supprimer le lien de façon propre
        removeLinkGraphData(graphDataPath, parentId, childId); // temporaire
    }
    refreshGraphPNG();
    ///addParent renvoie un Link* -> l'ajouter à la liste
}

void onToggleAddChild_cb(GtkWidget *checkBtn, gpointer child) {
    gboolean active = gtk_check_button_get_active(GTK_CHECK_BUTTON(checkBtn));
    char parentId[3], childId[3];
    sprintf(parentId, "%d", cardSelected->id);
    sprintf(childId, "%d", ((Card*)child)->id);
    if(active){
        addLinkGraphData(graphDataPath, parentId, childId, DEFAULT);
        addLinkBtn(addLink(curProject, cardSelected, ((Card*) child), DEFAULT), cardSelected, ((Card*) child));
    } else {
        //supprimer le lien de façon propre
        removeLinkGraphData(graphDataPath, parentId, childId); // temporaire
    }
    refreshGraphPNG();
    ///addChild renvoie un Link* -> l'ajouter à la liste
}

void closeProject(GtkApplication *app, gpointer user_data){
    if(curProject != NULL){
        char graphPNGPath[MAXPATH+10];
        strcpy(graphPNGPath, graphDirPath);
        strcat(graphPNGPath, "/graph.png");

        // remove the graph files

        if(remove(graphDataPath) != 0){
            fprintf(stderr, "error : deletion of the file %s is impossible\n",graphDataPath);
            fprintf(stderr,"%d\n", errno);
        }
        if(remove(graphPNGPath) != 0){
            fprintf(stderr, "error : deletion of the file %s is impossible\n", graphPNGPath);
            fprintf(stderr,"%d\n", errno);
        }

        // remove the graph directory

        if(rmdir(graphDirPath) != 0){
            fprintf( stderr, "error : deletion of the directory %s is impossible\n", graphDirPath);
            fprintf(stderr,"%d\n", errno);
        }

        deleteProject(curProject);
    }
}

