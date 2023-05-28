
#include <stdio.h>
#include <string.h>
#include <errno.h>


#include "../View/ViewApplication.h"
#include "../View/GraphHandler.h"

#include "../Model/VertexList.h"
#include "../Model/EdgeList.h"
#include "../Model/Card.h"
#include "../Model/Link.h"
#include "../Model/algo.h"

#include "Controller.h"

Project *curProject = NULL;
Card *selectedCard = NULL;
Link *selectedLink = NULL;
GtkWidget *selectedCardBtn = NULL;
GtkWidget *selectedLinkBtn = NULL;
extern GObject *secondWindowOpen;
extern GObject *cardBox;
extern GObject *linkBox;
bool unsavedChanges;

/* Main */

int main(int argc, char *argv[]) {
#ifdef GTK_SRCDIR
    g_chdir (GTK_SRCDIR);
#endif

    int status;

    GtkApplication *app = gtk_application_new(NULL, 0);
    g_signal_connect (app, "activate", G_CALLBACK(initView), NULL);

    status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return status;
}

/** * * Classic functions * * **/


/*void onCheckLoop(Project *p) {
    int result = checkLoops(p);
    if (result == 1) {
        printf("Loop detected in the project.\n");
        // Perform actions to handle the presence of a loop
        // ...
    } else if (result == 0) {
        printf("No loops found in the project.\n");
        // Perform actions when no loops are found
        // ...
    } else {
        fprintf(stderr, "Error: Project allocation failed.\n");
        // Perform error handling
        // ...
    }
} */

void unselectCard() {
    selectedCard = NULL;
    selectedCardBtn = NULL;
    refreshSelectedCardLabel();
    disableRightCardButtons();
}

void unselectLink() {
    selectedLink = NULL;
    selectedLinkBtn = NULL;
    refreshSelectedLinkLabel();
    disableRightLinkButtons();
}

void setCardFixNumber(int n) {
    if (n == -1) {
        unfixCardNumber(selectedCard);
        //change all links pointing toward the current card to default
        setOnFirstEdge(&(selectedCard->parents));
        while (!isOutOfListEdge(&(selectedCard->parents))) {
            setLinkType(selectedCard->parents.current->link, DEFAULT);
            changeLinkBtnTypeFromCards(selectedCard->parents.current->link->parent, selectedCard, DEFAULT);
            char parentId[3], childId[3];
            sprintf(parentId, "%d", selectedCard->parents.current->link->parent->id);
            sprintf(childId, "%d", selectedCard->id);
            modifyLinkTypeGraphData(parentId, childId, DEFAULT);
            setOnNextEdge(&(selectedCard->parents));
        }
        changeInfoLabel("Le numéro de la carte sélectionnée n'est plus fixé");
    } else {
        fixCardNumber(selectedCard, (char) n);
        //change all links pointing toward the current card to fixed number
        setOnFirstEdge(&(selectedCard->parents));
        while (!isOutOfListEdge(&(selectedCard->parents))) {
            setLinkType(selectedCard->parents.current->link, FIXED);
            changeLinkBtnTypeFromCards(selectedCard->parents.current->link->parent, selectedCard, FIXED);
            char parentId[3], childId[3];
            sprintf(parentId, "%d", selectedCard->parents.current->link->parent->id);
            sprintf(childId, "%d", selectedCard->id);
            modifyLinkTypeGraphData(parentId, childId, FIXED);
            setOnNextEdge(&(selectedCard->parents));
        }
        changeInfoLabel("Le numéro de la carte sélectionnée est maintenant fixé");
    }
    refreshGraphPNG();
}

void retrieveParentsChildren(GtkBox *parentsBox, GtkBox *childrenBox) {
    setOnFirstVertex(&(curProject->cardList));
    Card *c;
    GtkWidget *parentCheckBtn, *childCheckBtn;
    while (!isOutOfListVertex(&(curProject->cardList))) {
        c = curProject->cardList.current->card;
        if (c != selectedCard) {
            parentCheckBtn = gtk_check_button_new();
            childCheckBtn = gtk_check_button_new();
            addCardLblForModify(c, parentCheckBtn, GTK_WIDGET(parentsBox));
            addCardLblForModify(c, childCheckBtn, GTK_WIDGET(childrenBox));
            //vérification de l'existence des liens
            setOnFirstEdge(&(selectedCard->parents));
            while (!isOutOfListEdge(&(selectedCard->parents))) {
                if (selectedCard->parents.current->link->parent == c) {
                    gtk_check_button_set_active(GTK_CHECK_BUTTON(parentCheckBtn), true);
                }
                setOnNextEdge(&(selectedCard->parents));
            }
            setOnFirstEdge(&(selectedCard->children));
            while (!isOutOfListEdge(&(selectedCard->children))) {
                if (selectedCard->children.current->link->child == c) {
                    gtk_check_button_set_active(GTK_CHECK_BUTTON(childCheckBtn), true);
                }
                setOnNextEdge(&(selectedCard->children));
            }
            g_signal_connect(parentCheckBtn, "toggled", G_CALLBACK(onToggleAddParent), c);
            g_signal_connect(childCheckBtn, "toggled", G_CALLBACK(onToggleAddChild), c);
        }
        setOnNextVertex(&(curProject->cardList));
    }
}

void addLinkFromToggle(Card *parent, Card *child) {
    char parentId[3], childId[3];
    sprintf(parentId, "%d", parent->id);
    sprintf(childId, "%d", child->id);
    Link *l = NULL;
    if(child->fixedNumber == 1) {
        l = addLink(curProject, parent, child, FIXED);
    } else {
        l = addLink(curProject, parent, child, DEFAULT);
    }
    if (l != NULL) {
        addLinkGraphData(parentId, childId, l->type);
        addLinkBtn(l);
        char message[48];
        sprintf(message, "Le lien %d → %d a été ajouté avec succès.", parent->id, child->id);
        changeInfoLabel(message);
    } else {
        if (curProject->nbLinks == MAXLINK) {
            changeInfoLabel("Impossible d'ajouter un nouveau lien, le nombre maximum de liens a été atteint.");
        } else {
            changeInfoLabel("Une erreur est survenue pendant l'ajout du lien.\nVeuillez réessayer.");
        }
    }
}

void deleteLinkFromToggle(Card *parent, Card *child) {
    char parentId[3], childId[3];
    sprintf(parentId, "%d", parent->id);
    sprintf(childId, "%d", child->id);
    if (selectedLink != NULL) {
        if (selectedLink->parent == parent && selectedLink->child == child) {
            unselectLink();
        }
    }
    deleteLinkFromCards(curProject, parent, child);
    removeLinkGraphData(parentId, childId);
    removeLinkBtnFromCards(parent, child);
    char message[50];
    sprintf(message, "Le lien %d → %d a été supprimé avec succès.",parent->id , child->id);
    changeInfoLabel(message);
}

/** * * Callbacks * * **/

/* Project managing callbacks */

void onConfirmNewProject(GtkWidget *newProjectWindow) {
    GtkWidget *fileChooser = gtk_widget_get_first_child(gtk_widget_get_first_child(newProjectWindow));
    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(fileChooser));
    GtkWidget *nameEntry = gtk_widget_get_next_sibling(
            gtk_widget_get_next_sibling(gtk_widget_get_first_child(gtk_widget_get_first_child(newProjectWindow))));
    if (file != NULL && strcmp(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(nameEntry))), "") != 0) {

        char newFilePath[MAXPATH + 2 * MAXNAME + 7];
        sprintf(newFilePath, "%s/%s/%s.txt", g_file_get_path(file), gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(nameEntry))), gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(nameEntry))));
        FILE *fp;
        if ((fp = fopen(newFilePath, "r")) == NULL) {
            if (errno == 2) {
                curProject = allocProject();

                if (curProject != NULL) {

                    /*Temporary projet path and name, to change depending on OS*/
                    initProject(curProject, g_file_get_path(file),
                                (char *) gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(nameEntry))));
                    initGraphFiles(curProject);
                    refreshCardBoxLabel();
                    refreshLinkBoxLabel();

                    gtk_window_close(GTK_WINDOW(newProjectWindow));
                    unsavedChanges = false;
                    changeInfoLabel("Bienvenue dans UnlockMaker ! Commencez votre projet en ajoutant une carte");
                } else {
                    gtk_label_set_label(GTK_LABEL(gtk_widget_get_next_sibling(
                                                gtk_widget_get_first_child(gtk_widget_get_first_child(GTK_WIDGET(newProjectWindow))))),
                                        " Erreur : Impossible de créer le projet");
                    fprintf(stderr, "error : creation of the project is impossible\n");
                    fprintf(stderr, "%d\n", errno);
                }
            }
        } else {
            gtk_label_set_label(GTK_LABEL(gtk_widget_get_next_sibling(
                                        gtk_widget_get_first_child(gtk_widget_get_first_child(GTK_WIDGET(newProjectWindow))))),
                                " Erreur : un projet avec ce nom existe déjà dans le dossier sélectionné");
            fclose(fp);
        }
    }
}

void onConfirmOpenProject(GtkWidget *openProjectWindow) {
    GtkWidget *fileChooser = gtk_widget_get_first_child(gtk_widget_get_first_child(openProjectWindow));
    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(fileChooser));
    if (file != NULL) {
        if (g_file_info_get_file_type(g_file_query_info(file, "standard::", G_FILE_QUERY_INFO_NONE, NULL, NULL)) ==
            G_FILE_TYPE_DIRECTORY) {
            gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fileChooser), file, NULL);
        } else {
            if ((curProject = loadProject(g_file_get_path(g_file_get_parent(g_file_get_parent(file))),
                                          g_file_get_basename(g_file_get_parent(file)))) != NULL) {

                /*int loopResult = checkLoops(curProject);
                if (loopResult == 1) {
                    // Loop detected in the project
                    printf("Loop detected in the project.\n");
                    // Perform actions to handle the presence of a loop
                    // ...
                } else if (loopResult == 0) {
                    // No loops found in the project
                    printf("No loops found in the project.\n");
                    // Proceed with initializing and displaying the project
                    gtk_window_close(GTK_WINDOW(openProjectWindow));
                    // Rest of the code...
                } else {
                    fprintf(stderr, "Error: Project allocation failed.\n");
                    // Perform error handling
                    // ...
                }*/
                gtk_window_close(GTK_WINDOW(openProjectWindow));
                // création des boutons des cartes et des liens existants + mise à jour du graph
                initGraphFiles(curProject);
                char cardId[3];
                setOnFirstVertex(&(curProject->cardList));
                while (!isOutOfListVertex(&(curProject->cardList))) {
                    addCardBtn(curProject->cardList.current->card);
                    sprintf(cardId, "%d", curProject->cardList.current->card->id);
                    addCardGraphData(cardId, curProject->cardList.current->card->type);
                    if (curProject->root == curProject->cardList.current->card) {
                        setCardAsRootGraphData(cardId);
                    }
                    setOnNextVertex(&(curProject->cardList));
                }
                char cardId2[3];
                setOnFirstEdge(&(curProject->linkList));
                while (!isOutOfListEdge(&(curProject->linkList))) {
                    addLinkBtn(curProject->linkList.current->link);
                    sprintf(cardId, "%d", curProject->linkList.current->link->parent->id);
                    sprintf(cardId2, "%d", curProject->linkList.current->link->child->id);
                    addLinkGraphData(cardId, cardId2, curProject->linkList.current->link->type);
                    setOnNextEdge(&(curProject->linkList));
                }
                refreshGraphPNG();
                refreshCardBoxLabel();
                refreshLinkBoxLabel();
            } else {
                gtk_label_set_label(GTK_LABEL(gtk_widget_get_next_sibling(
                                            gtk_widget_get_first_child(gtk_widget_get_first_child(GTK_WIDGET(openProjectWindow))))),
                                    " Erreur : le fichier ouvert n'a pas un format valide");
            }
        }
    }
    unsavedChanges = false;
    changeInfoLabel("Bon retour dans UnlockMaker !");
}

void onSaveProject() {
    if (saveProject(curProject) == 0) {
        char message[MAXPATH+MAXNAME+55];
        sprintf(message, "Projet enregistré avec succès.\nChemin du projet : %s/%s", curProject->path, curProject->name);
        changeInfoLabel(message);
        unsavedChanges = false;
    } else {
        changeInfoLabel("Une erreur est survenue pendant l'enregistrement du projet.\nVeuillez réessayer.");
    }
}

void onCloseNoSave() {
    deleteGraphFiles();
    char saveFilePath[MAXPATH + 2 * MAXNAME + 7];
    sprintf(saveFilePath, "%s/%s/%s.txt", curProject->path, curProject->name, curProject->name);
    FILE *fp;
    if ((fp = fopen(saveFilePath, "r")) == NULL) {
        if (errno == 2) deleteProject(curProject);
    } else {
        fclose(fp);
    }
}

void onConfirmImportTopImage(GtkWidget *importImageWindow) {
    GtkWidget *fileChooser = gtk_widget_get_first_child(gtk_widget_get_first_child(importImageWindow));
    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(fileChooser));
    if (file != NULL) {
        if (g_file_info_get_file_type(g_file_query_info(file, "standard::", G_FILE_QUERY_INFO_NONE, NULL, NULL)) ==
            G_FILE_TYPE_DIRECTORY) {
            gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fileChooser), file, NULL);
        } else {
            gtk_window_close(GTK_WINDOW(importImageWindow));
            setTopImage(curProject, g_file_get_path(file));
        }
    }
    changeInfoLabel("Une nouvelle image pour le haut des cartes a bien été importée.");
    unsavedChanges = true;
}

void onConfirmImportBottomImage(GtkWidget *importImageWindow) {
    GtkWidget *fileChooser = gtk_widget_get_first_child(gtk_widget_get_first_child(importImageWindow));
    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(fileChooser));
    if (file != NULL) {
        if (g_file_info_get_file_type(g_file_query_info(file, "standard::", G_FILE_QUERY_INFO_NONE, NULL, NULL)) ==
            G_FILE_TYPE_DIRECTORY) {
            gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fileChooser), file, NULL);
        } else {
            gtk_window_close(GTK_WINDOW(importImageWindow));
            setBottomImage(curProject, g_file_get_path(file));
        }
    }
    changeInfoLabel("Une nouvelle image pour le haut des cartes a bien été importée.");
    unsavedChanges = true;
}

void onConfirmImportBackImage(GtkWidget *importImageWindow) {
    GtkWidget *fileChooser = gtk_widget_get_first_child(gtk_widget_get_first_child(importImageWindow));
    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(fileChooser));
    if (file != NULL) {
        if (g_file_info_get_file_type(g_file_query_info(file, "standard::", G_FILE_QUERY_INFO_NONE, NULL, NULL)) ==
            G_FILE_TYPE_DIRECTORY) {
            gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fileChooser), file, NULL);
        } else {
            gtk_window_close(GTK_WINDOW(importImageWindow));
            setBackImage(curProject, g_file_get_path(file));
        }
    }
    changeInfoLabel("Une nouvelle image pour le haut des cartes a bien été importée.");
    unsavedChanges = true;
}

/* Card managing callbacks */

void onAddCard() {
    if (secondWindowOpen != NULL) {
        gtk_window_close(GTK_WINDOW(secondWindowOpen));
    }
    Card *c = addEmptyCard(curProject);
    if (c != NULL) {
        addCardBtn(c);
        char cardId[3];
        sprintf(cardId, "%d", c->id);
        addCardGraphData(cardId, c->type);
        refreshGraphPNG();
        refreshCardBoxLabel();
        char message[45];
        sprintf(message, "La carte n°%d a été ajoutée avec succès.", c->id);
        changeInfoLabel(message);
        unsavedChanges = true;
    } else {
        if(curProject->nbCards == MAXCARD) {
            changeInfoLabel("Impossible d'ajouter une nouvelle carte, le nombre maximum de cartes a été atteint.");
        } else {
            changeInfoLabel("Une erreur est survenue pendant l'ajout de la carte.\nVeuillez réessayer.");
        }
    }
}

void onSelectCard(GtkWidget *cardBtn, Card *card) {
    char unselectedCardNames[4][23] = {"unselectedGreyCardBtn", "unselectedBlueCardBtn", "unselectedRedCardBtn",
                                       "unselectedGreenCardBtn"};
    char selectedCardNames[4][21] = {"selectedGreyCardBtn", "selectedBlueCardBtn", "selectedRedCardBtn",
                                     "selectedGreenCardBtn"};
    gboolean reopenSecondWindow = false;
    if (secondWindowOpen != NULL) {
        gtk_window_close(GTK_WINDOW(secondWindowOpen));
        reopenSecondWindow = true;
    }
    if (selectedCard != NULL) {
        gtk_widget_set_name(selectedCardBtn, unselectedCardNames[selectedCard->type]);
    }
    selectedCard = card;
    selectedCardBtn = cardBtn;
    gtk_widget_set_name(selectedCardBtn, selectedCardNames[selectedCard->type]);
    if (reopenSecondWindow) openModifyCardWindow();
    enableRightCardButtons();
    refreshSelectedCardLabel();
}

void onModifyCardType(GtkWidget *dropDown) {
    if (selectedCard != NULL) {
        int newType = (int) gtk_drop_down_get_selected(GTK_DROP_DOWN(dropDown));
        char cardId[3];
        sprintf(cardId, "%d", selectedCard->id);
        modifyCardTypeGraphData(cardId, newType);
        refreshGraphPNG();
        setCardType(selectedCard, newType);
        char cardNames[4][23] = {"selectedGreyCardBtn", "selectedBlueCardBtn", "selectedRedCardBtn",
                                 "selectedGreenCardBtn"};
        gtk_widget_set_name(selectedCardBtn, cardNames[newType]);
        changeInfoLabel("Le type de la carte sélectionnée a été modifié");
        unsavedChanges = true;
    }
}

void onToggleFixedNumberCheck(GtkWidget *checkButton, GtkWidget *entry) {
    gboolean active = gtk_check_button_get_active(GTK_CHECK_BUTTON(checkButton));
    if (!active) {
        setCardFixNumber(-1);
        gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(entry)))), "", 0);
    } else {
        setCardFixNumber((int)strtol(gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(entry))))), NULL, 10));
    }
    gtk_widget_set_can_target(GTK_WIDGET(entry), active);
    unsavedChanges = true;
}

void onEnterCardFixedNumber(GtkWidget *entry, GtkWidget *checkButton) {
    if (gtk_check_button_get_active(GTK_CHECK_BUTTON(checkButton))) {
        setCardFixNumber((int) strtol(
                gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(entry))))), NULL,
                10));
    }
    changeInfoLabel("Le numéro de la carte sélectionnée a été modifié");
}

void onToggleSetCardAsRoot(GtkWidget *checkButton) {
    char cardId[3] = "";
    sprintf(cardId, "%d", selectedCard->id);
    if (gtk_check_button_get_active(GTK_CHECK_BUTTON(checkButton))) {
        setCardAsRootGraphData(cardId);
        setRoot(curProject, selectedCard);
    } else {
        removeCardsAsRootGraphData();
        setRoot(curProject, NULL);
    }
    refreshGraphPNG();
    changeInfoLabel("La carte sélectionnée est maintenant la racine du projet");
    unsavedChanges = true;
}

void onConfirmImportCardImage(GtkWidget *importImageWindow) {
    GtkWidget *fileChooser = gtk_widget_get_first_child(gtk_widget_get_first_child(importImageWindow));
    GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(fileChooser));
    if (file != NULL) {
        if (g_file_info_get_file_type(g_file_query_info(file, "standard::", G_FILE_QUERY_INFO_NONE, NULL, NULL)) ==
            G_FILE_TYPE_DIRECTORY) {
            gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fileChooser), file, NULL);
        } else {
            gtk_window_close(GTK_WINDOW(importImageWindow));
            setCardImage(curProject, selectedCard, g_file_get_path(file));
        }
    }
    changeInfoLabel("Une nouvelle image pour la carte sélectionnée a bien été importée.");
    unsavedChanges = true;
}

void onToggleAddParent(GtkWidget *parentCheckBtn, Card *parentCard) {
    gboolean active = gtk_check_button_get_active(GTK_CHECK_BUTTON(parentCheckBtn));
    if (active) {
        addLinkFromToggle(parentCard, selectedCard);
    } else {
        deleteLinkFromToggle(parentCard, selectedCard);
    }
    refreshGraphPNG();
    refreshLinkBoxLabel();
    unsavedChanges = true;
}

void onToggleAddChild(GtkWidget *childCheckBtn, Card *childCard) {
    gboolean active = gtk_check_button_get_active(GTK_CHECK_BUTTON(childCheckBtn));
    if (active) {
        addLinkFromToggle(selectedCard, childCard);
    } else {
        deleteLinkFromToggle(selectedCard, childCard);
    }
    refreshGraphPNG();
    refreshLinkBoxLabel();
    unsavedChanges = true;
}

void onPressDeleteCard() {
    if (selectedCard != NULL) {
        if (secondWindowOpen != NULL) {
            gtk_window_close(GTK_WINDOW(secondWindowOpen));
        }
        char cardId[3];
        sprintf(cardId, "%d", selectedCard->id);

        //link deletion
        setOnFirstEdge(&selectedCard->parents);
        while (!isOutOfListEdge(&selectedCard->parents)) {
            removeLinkBtnFromCards(selectedCard->parents.current->link->parent, selectedCard);
            if (selectedLink == selectedCard->parents.current->link) {
                unselectLink();
            }
            setOnNextEdge(&selectedCard->parents);
        }
        setOnFirstEdge(&selectedCard->children);
        while (!isOutOfListEdge(&selectedCard->children)) {
            removeLinkBtnFromCards(selectedCard, selectedCard->children.current->link->child);
            if (selectedLink == selectedCard->children.current->link) {
                unselectLink();
            }
            setOnNextEdge(&selectedCard->children);
        }
        deleteCard(curProject, selectedCard);
        gtk_box_remove(GTK_BOX(cardBox), selectedCardBtn);
        unselectCard();

        removeCardGraphData(cardId);
        refreshGraphPNG();
        refreshCardBoxLabel();
        refreshLinkBoxLabel();
        char message[63];
        sprintf(message, "La carte n°%s et ses liens ont été supprimés avec succès.", cardId);
        changeInfoLabel(message);
        unsavedChanges = true;
    }
}

/* Link managing callbacks */

void onSelectLink(GtkWidget *linkBtn, Link *link) {
    char unselectedLinkNames[4][23] = {"unselectedFoundLinkBtn", "unselectedCombLinkBtn", "unselectedHintLinkBtn",
                                       "unselectedFixedLinkBtn"};
    char selectedLinkNames[4][21] = {"selectedFoundLinkBtn", "selectedCombLinkBtn", "selectedHintLinkBtn",
                                     "selectedFixedLinkBtn"};
    gboolean reopenSecondWindow = false;
    if (secondWindowOpen != NULL) {
        gtk_window_close(GTK_WINDOW(secondWindowOpen));
        reopenSecondWindow = true;
    }
    if (selectedLink != NULL) {
        gtk_widget_set_name(selectedLinkBtn, unselectedLinkNames[selectedLink->type]);
    }
    selectedLink = (Link *) link;
    selectedLinkBtn = linkBtn;
    gtk_widget_set_name(selectedLinkBtn, selectedLinkNames[selectedLink->type]);
    if (reopenSecondWindow) openModifyLinkWindow();
    enableRightLinkButtons();
    refreshSelectedLinkLabel();
}

void onModifyLinkType(GtkWidget *dropDown) {
    if (selectedLink != NULL) {
        int newType = (int) gtk_drop_down_get_selected(GTK_DROP_DOWN(dropDown));
        setLinkType(selectedLink, newType);
        char linkNames[4][23] = {"selectedFoundLinkBtn", "selectedCombLinkBtn","selectedHintLinkBtn"};
        gtk_widget_set_name(selectedLinkBtn, linkNames[newType]);
        char parentId[3], childId[3];
        sprintf(parentId, "%d", selectedLink->parent->id);
        sprintf(childId, "%d", selectedLink->child->id);
        modifyLinkTypeGraphData(parentId, childId, newType);
        refreshGraphPNG();
        changeInfoLabel("Le type du lien sélectionné a été modifié");
        unsavedChanges = true;
    }
}

void onPressDeleteLink() {
    if (selectedLink != NULL) {
        if (secondWindowOpen != NULL) {
            gtk_window_close(GTK_WINDOW(secondWindowOpen));
        }
        char parentId[3], childId[3];
        sprintf(parentId, "%d", selectedLink->parent->id);
        sprintf(childId, "%d", selectedLink->child->id);
        deleteLink(curProject, selectedLink);
        gtk_box_remove(GTK_BOX(linkBox), selectedLinkBtn);
        unselectLink();
        removeLinkGraphData(parentId, childId);
        refreshGraphPNG();
        refreshLinkBoxLabel();
        char message[51];
        sprintf(message, "Le lien %s → %s a été supprimé avec succès.", parentId, childId);
        changeInfoLabel(message);
        unsavedChanges = true;
    }
}

/* Image managing callbacks */

void onChangeImageZoom(GtkWidget *zoomBtn, gpointer image) {
    gtk_widget_set_size_request(GTK_WIDGET(image),
                                1130 + 20 * (int) gtk_scale_button_get_value(GTK_SCALE_BUTTON(zoomBtn)),
                                941 + 20 * (int) gtk_scale_button_get_value(GTK_SCALE_BUTTON(zoomBtn)));
}

