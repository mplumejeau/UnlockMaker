/*
// Created by vboxuser on 01/05/23.
*/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../View/ViewApplication.h"
#include "../View/GraphHandler.h"

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
extern GObject *secondWindowOpen;

int main(int argc, char* argv[]) {
#ifdef GTK_SRCDIR
    g_chdir (GTK_SRCDIR);
#endif

    int status;

    GtkApplication *app = gtk_application_new(NULL, 0);
    g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return status;
}

void createNewProject_cb(){

    curProject = allocProject();

    if(curProject != NULL) {
        /*Temporary projet path and name, to change depending on OS*/
        initProject(curProject, (char*)g_get_tmp_dir(), "Sans Titre");
        initGraphFiles(curProject);
    } else {
        fprintf(stderr, "error : creation of the project is impossible\n");
        fprintf(stderr,"%d\n", errno);
    }
}

void onConfirmNewProject_cb(GtkWidget *saveProjectWindow) {
    GtkWidget *fileChooser = gtk_widget_get_first_child(gtk_widget_get_first_child(saveProjectWindow));
    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(fileChooser));
    GtkWidget *nameEntry = gtk_widget_get_next_sibling(gtk_widget_get_next_sibling(gtk_widget_get_first_child(gtk_widget_get_first_child(saveProjectWindow))));
    if(file != NULL && strcmp(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(nameEntry))), "") != 0) {
        curProject = allocProject();

        if(curProject != NULL) {
            /*Temporary projet path and name, to change depending on OS*/
            initProject(curProject, g_file_get_path(file), (char*)gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(nameEntry))));
            initGraphFiles(curProject);
        } else {
            fprintf(stderr, "error : creation of the project is impossible\n");
            fprintf(stderr,"%d\n", errno);
        }
    }
    destroyWindow_cb(GTK_WINDOW(saveProjectWindow));
}

void onAddCard_cb(gpointer box) {
    if(secondWindowOpen!=NULL) {
        destroyWindow_cb(GTK_WINDOW(secondWindowOpen));
    }
    Card* c = addEmptyCard(curProject);
    if(c!=NULL) {
        addCardBtn(c, GTK_WIDGET(box));
        char cardId[3];
        sprintf(cardId, "%d", c->id);
        addCardGraphData(cardId, GREY);
        refreshGraphPNG();
    } else {
        ///TODO : Message d'erreur en bas à droite : Nombre de cartes maximum atteint (60)
    }
}

void onSelectCard_cb(GtkWidget *cardBtn, gpointer card) {
    char unselectedCardNames[4][23] = {"UnselectedGreyCardBtn", "UnselectedBlueCardBtn", "UnselectedRedCardBtn", "UnselectedGreenCardBtn"};
    char selectedCardNames[4][21] = {"SelectedGreyCardBtn", "SelectedBlueCardBtn", "SelectedRedCardBtn", "SelectedGreenCardBtn"};
    if(cardSelected != NULL) {
        gtk_widget_set_name(cardSelectedBtn, unselectedCardNames[cardSelected->type]);
    }
    cardSelected = (Card*)card;
    cardSelectedBtn = cardBtn;
    gtk_widget_set_name(cardSelectedBtn, selectedCardNames[cardSelected->type]);
    char panelLabel[26] = "Carte sélectionnée : ";
    char cardId[3];
    sprintf(cardId, "%d", ((Card*)card)->id);
    strcat(panelLabel,cardId);
    modifyInfoPanel(panelLabel);
    if(secondWindowOpen!=NULL) {
        openModifyCardWindow_cb();
    }
    enableRightCardButtons();
}

void unselectCard() {
    cardSelected = NULL;
    cardSelectedBtn = NULL;
    modifyInfoPanel("Aucune carte selectionnée");
    disableRightCardButtons();
}

void onSelectLink_cb(GtkWidget *linkBtn, gpointer link) {
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
    if(secondWindowOpen!=NULL) {
        openModifyLinkWindow_cb();
    }
    enableRightLinkButtons();
}

void unselectLink() {
    linkSelected = NULL;
    linkSelectedBtn = NULL;
    //modifyInfoPanel("Aucune carte selectionnée");
    disableRightLinkButtons();
}

void onPressDeleteCard_cb(gpointer box) {
    if(cardSelected!=NULL) {
        if(secondWindowOpen!=NULL) {
            destroyWindow_cb(GTK_WINDOW(secondWindowOpen));
        }
        char cardId[3];
        sprintf(cardId, "%d", cardSelected->id);

        //link deletion
        setOnFirstEdge(&cardSelected->parents);
        while(!isOutOfListEdge(&cardSelected->parents)) {
            removeLinkBtnFromCards(cardSelected->parents.current->link->parent, cardSelected);
            if(linkSelected == cardSelected->parents.current->link) {
                unselectLink();
            }
            setOnNextEdge(&cardSelected->parents);
        }
        setOnFirstEdge(&cardSelected->children);
        while(!isOutOfListEdge(&cardSelected->children)) {
            removeLinkBtnFromCards(cardSelected, cardSelected->children.current->link->child);
            if(linkSelected == cardSelected->children.current->link) {
                unselectLink();
            }
            setOnNextEdge(&cardSelected->children);
        }
        deleteCard(curProject, cardSelected);
        gtk_box_remove(GTK_BOX(box), cardSelectedBtn);
        unselectCard();

        removeCardGraphData(cardId);
        refreshGraphPNG();
    }
}

void onPressDeleteLink_cb(gpointer box) {
    if(linkSelected!=NULL) {
        if(secondWindowOpen!=NULL) {
            destroyWindow_cb(GTK_WINDOW(secondWindowOpen));
        }
        char parentId[3], childId[3];
        sprintf(parentId, "%d", linkSelected->parent->id);
        sprintf(childId, "%d", linkSelected->child->id);
        removeLinkGraphData(parentId, childId);
        refreshGraphPNG();
        deleteLink(curProject, linkSelected);
        gtk_box_remove(GTK_BOX(box), linkSelectedBtn);
        unselectLink();
    }
}

void onModifyCardType_cb(int newType) {
    if(cardSelected != NULL) {
        char cardId[3];
        sprintf(cardId, "%d", cardSelected->id);
        modifyCardTypeGraphData(cardId, newType);
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
        modifyLinkTypeGraphData(parentId, childId, newType);
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
        onToggleCardFixedNumber_cb((int)strtol(gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(entry))))),NULL, 10));
    }
}

void onToggleCardRoot_cb(GtkWidget *checkButton) {
    char cardId[3]="";
    sprintf(cardId, "%d", cardSelected->id);
    if(gtk_check_button_get_active(GTK_CHECK_BUTTON(checkButton))) {
        setCardAsRootGraphData(cardId);
        setRoot(curProject, cardSelected);
    } else {
        removeCardAsRootGraphData();
        setRoot(curProject, NULL);
    }
    refreshGraphPNG();

}

void retrieveParentsChildren(GtkBox *parentsBox, GtkBox *childrenBox) {
    setOnFirstVertex(&(curProject->cardList));
    Card* c;
    GtkWidget *checkBtnParent, *checkBtnChild;
    while(!isOutOfListVertex(&(curProject->cardList))) {
        c=curProject->cardList.current->card;
        if(c!=cardSelected) {
            checkBtnParent = gtk_check_button_new();
            checkBtnChild = gtk_check_button_new();
            addCardLblForModify(c, checkBtnParent, GTK_WIDGET(parentsBox));
            addCardLblForModify(c, checkBtnChild, GTK_WIDGET(childrenBox));
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
    }
}

void onToggleAddParent_cb(GtkWidget *checkBtn, gpointer parent) {
    gboolean active = gtk_check_button_get_active(GTK_CHECK_BUTTON(checkBtn));
    char parentId[3], childId[3];
    sprintf(parentId, "%d", ((Card*)parent)->id);
    sprintf(childId, "%d", cardSelected->id);
    if(active){
        Link* l = addLink(curProject, (Card*) parent, cardSelected, DEFAULT);
        if(l!=NULL) {
            addLinkGraphData(parentId, childId, DEFAULT);
            addLinkBtn(l);
        } else {
            ///TODO : Message d'erreur en bas à droite : Nombre de liens maximum atteint (200)
        }
    } else {
        if(linkSelected!=NULL) {
            if(linkSelected->parent == (Card*)parent && linkSelected->child == cardSelected) {
                unselectLink();
            }
        }
        deleteLinkFromCards(curProject, (Card*)parent, cardSelected);
        removeLinkGraphData(parentId, childId);
        removeLinkBtnFromCards(parent, cardSelected);
    }
    refreshGraphPNG();
}

void onToggleAddChild_cb(GtkWidget *checkBtn, gpointer child) {
    gboolean active = gtk_check_button_get_active(GTK_CHECK_BUTTON(checkBtn));
    char parentId[3], childId[3];
    sprintf(parentId, "%d", cardSelected->id);
    sprintf(childId, "%d", ((Card *) child)->id);
    if (active) {
        addLinkGraphData(parentId, childId, DEFAULT);
        Link *l = addLink(curProject, cardSelected, (Card *) child, DEFAULT);
        if (l != NULL) {
            addLinkBtn(l);
        } else {
            ///TODO : Message d'erreur en bas à droite : Nombre de liens maximum atteint (200)
        }
    } else {
        if(linkSelected != NULL) {
            if(linkSelected->parent == cardSelected && linkSelected->child == (Card*)child) {
                unselectLink();
            }
        }
        deleteLinkFromCards(curProject, cardSelected, (Card *) child);
        removeLinkGraphData(parentId, childId);
        removeLinkBtnFromCards(cardSelected, (Card*)child);
    }
    refreshGraphPNG();
}

void changeImageZoom_cb(GtkWidget *zoomBtn, gpointer image) {
    gtk_widget_set_size_request(GTK_WIDGET(image), 1130+20*(int)gtk_scale_button_get_value(GTK_SCALE_BUTTON(zoomBtn)), 941+20*(int)gtk_scale_button_get_value(GTK_SCALE_BUTTON(zoomBtn)));
}

void onConfirmOpenProject_cb(GtkWidget *openProjectWindow) {
    GtkWidget *fileChooser = gtk_widget_get_first_child(gtk_widget_get_first_child(openProjectWindow));
    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(fileChooser));
    if(file != NULL) {
        if ((curProject = loadProject(g_file_get_path(g_file_get_parent(g_file_get_parent(file))), g_file_get_basename(g_file_get_parent(file)))) != NULL) {
            destroyWindow_cb(GTK_WINDOW(openProjectWindow));
            ///TODO : créer les boutons des cartes et liens existants + graph
        } else {
            gtk_label_set_label(GTK_LABEL(gtk_widget_get_next_sibling(gtk_widget_get_first_child(gtk_widget_get_first_child(GTK_WIDGET(openProjectWindow))))), " Erreur : le fichier ouvert n'a pas un format valide");
        }
    }
}

void onConfirmSaveProjectAs_cb(GtkWidget *saveProjectWindow) {
    GtkWidget *fileChooser = gtk_widget_get_first_child(gtk_widget_get_first_child(saveProjectWindow));
    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(fileChooser));
    GtkWidget *nameEntry = gtk_widget_get_next_sibling(gtk_widget_get_next_sibling(gtk_widget_get_first_child(gtk_widget_get_first_child(saveProjectWindow))));
    printf("Name : %s\n", gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(nameEntry))));
    if(file != NULL && strcmp(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(nameEntry))), "") != 0) {
        printf("Valid name and path. Path : %s\n", g_file_get_path(file));
        /* TODO : BESOIN D'UNE FONCTION saveProjectAs(char* newPath, char* newName) POUR CONTINUER (et adapter, + ajouter graph)
        char newPath[strlen(g_file_get_path(file))+strlen(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(nameEntry))))+2];
        strcpy(newPath, g_file_get_path(file));
        strcat(newPath, "/");
        strcat(newPath, gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(nameEntry))));
        if ( mkdir(newPath, 0755 ) != 0 && errno != 17) {
            fprintf( stderr, "error : creation of the directory %s is impossible\n", newPath);
            fprintf(stderr,"%d\n", errno);
        } else {
            strcpy(curProject->path, g_file_get_path(file));
            strcpy(curProject->name, gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(nameEntry))));
            saveProjectAs(curProject);

        }*/
        destroyWindow_cb(GTK_WINDOW(saveProjectWindow));
    }
}

void onPressSaveProject_cb() {
    if(strcmp(curProject->path, "/tmp") == 0) {
        onPressSaveProjectAs_cb();
    } else {
        saveProject(curProject);
    }
}

void onPressSaveProjectAs_cb() {

    /* Construct a GtkBuilder instance and load our UI description */
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "../View/ViewBuilder.ui", NULL);

    /* Connect signal handlers to the constructed widgets. */
    GObject *saveProjectWindow, *fileChooser, *abortBtn, *confirmBtn, *nameEntry;
    saveProjectWindow = gtk_builder_get_object(builder, "saveProjectWindow");

    fileChooser = gtk_builder_get_object(builder, "saveProjectBrowser");
    char path[MAXPATH] = "/home/";
    strcat(path, getlogin());
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fileChooser), g_file_new_for_path(path), NULL);

    nameEntry = gtk_builder_get_object(builder, "saveProjectEntry");
    gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(nameEntry)), curProject->name, (int)strlen(curProject->name));

    abortBtn = gtk_builder_get_object(builder, "saveProjectAbort");
    g_signal_connect_swapped(abortBtn, "clicked", G_CALLBACK(destroyWindow_cb), saveProjectWindow);

    confirmBtn = gtk_builder_get_object(builder, "saveProjectConfirm");
    g_signal_connect_swapped(confirmBtn, "clicked", G_CALLBACK(onConfirmSaveProjectAs_cb), saveProjectWindow);

    gtk_window_set_default_size(GTK_WINDOW(saveProjectWindow), 800, 500);

    gtk_widget_show(GTK_WIDGET (saveProjectWindow));
    g_object_unref(builder);
}

