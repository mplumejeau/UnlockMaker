
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>

#include "ViewApplication.h"
#include "GraphHandler.h"

#include "../Controller/Controller.h"

char imagePath[MAXPATH + 16] = "";
extern Card *selectedCard;
extern Link *selectedLink;
extern Project *curProject;
extern bool unsavedChanges;

/* Some widgets we need a global pointer to */
GObject *cardBoxLabel = NULL;
GObject *cardBox = NULL;
GObject *linkBoxLabel = NULL;
GObject *linkBox = NULL;

GObject *centerImage = NULL;

GObject *modifyCardBtn = NULL;
GObject *deleteCardBtn = NULL;
GObject *modifyLinkBtn = NULL;
GObject *deleteLinkBtn = NULL;

GObject *selectedCardLabel = NULL;
GObject *selectedLinkLabel = NULL;
GObject *infoLabel = NULL;

GObject *secondWindowOpen = NULL;
GObject *imageViewerWindow = NULL;

/* View initialization functions */

void initView(GtkApplication *app) {

    GObject *window, *button;
    loadCss();

    /* Construct a GtkBuilder instance and load our UI description */
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "../View/ViewBuilder.ui", NULL);

    /* Connect signal handlers to the constructed widgets */
    window = gtk_builder_get_object(builder, "mainWindow");
    gtk_window_set_default_size(GTK_WINDOW(window), 1700, 900);
    gtk_window_set_application(GTK_WINDOW(window), app);
    g_signal_connect(GTK_WINDOW(window), "close-request", G_CALLBACK(openCloseConfirmationWindow), NULL);

    button = gtk_builder_get_object(builder, "saveBtn");
    g_signal_connect(button, "clicked", G_CALLBACK(onSaveProject), NULL);

    button = gtk_builder_get_object(builder, "BtnAddCard");
    g_signal_connect(button, "clicked", G_CALLBACK(onAddCard), NULL);

    button = gtk_builder_get_object(builder, "zoomBtn");
    g_signal_connect(button, "value-changed", G_CALLBACK(onChangeImageZoom), centerImage);

    modifyCardBtn = gtk_builder_get_object(builder, "BtnModifyCard");
    g_signal_connect(modifyCardBtn, "clicked", G_CALLBACK(openModifyCardWindow), NULL);

    cardBox = gtk_builder_get_object(builder, "cardBox");
    deleteCardBtn = gtk_builder_get_object(builder, "BtnDeleteCard");
    g_signal_connect(deleteCardBtn, "clicked", G_CALLBACK(onPressDeleteCard), NULL);

    modifyLinkBtn = gtk_builder_get_object(builder, "BtnModifyLink");
    g_signal_connect(modifyLinkBtn, "clicked", G_CALLBACK(openModifyLinkWindow), NULL);

    linkBox = gtk_builder_get_object(builder, "linkBox");
    deleteLinkBtn = gtk_builder_get_object(builder, "BtnDeleteLink");
    g_signal_connect(deleteLinkBtn, "clicked", G_CALLBACK(onPressDeleteLink), NULL);

    button = gtk_builder_get_object(builder, "BtnModifyProject");
    g_signal_connect(button, "clicked", G_CALLBACK(openModifyProjectWindow), NULL);

    disableRightCardButtons();
    disableRightLinkButtons();

    /* Save pointers to other important widgets for later */
    cardBoxLabel = gtk_builder_get_object(builder, "cardBoxLabel");
    linkBoxLabel = gtk_builder_get_object(builder, "linkBoxLabel");
    selectedCardLabel = gtk_builder_get_object(builder, "selectedCardLabel");
    selectedLinkLabel = gtk_builder_get_object(builder, "selectedLinkLabel");
    infoLabel = gtk_builder_get_object(builder, "infoLabel");
    centerImage = gtk_builder_get_object(builder, "graphViewer");

    secondWindowOpen = NULL;
    openStartingWindow(GTK_WINDOW(window));

    g_object_unref(builder);
}

void loadCss(void) {
    /* Creates a pointer to the file containing the CSS descriptions */
    const gchar *css_style_file = "../View/ViewStyle.css";
    GFile *css_fp = g_file_new_for_path(css_style_file);

    /* Initializes a CSSProvider, adds the CSS descriptions to it, then applies them to the project */
    GtkCssProvider *CSSprovider = gtk_css_provider_new();
    gtk_css_provider_load_from_file(CSSprovider, css_fp);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(CSSprovider),
                                               GTK_STYLE_PROVIDER_PRIORITY_USER);

    /* Frees the CSSProvider */
    g_object_unref(CSSprovider);
}

/* Widget adding/removing functions */

void addCardBtn(Card *c) {
    char btnLabel[11] = "";
    sprintf(btnLabel, "Carte n°%d", c->id);
    GtkWidget *button = gtk_button_new_with_label(btnLabel);
    char cardNames[4][23] = {"unselectedGreyCardBtn", "unselectedBlueCardBtn", "unselectedRedCardBtn",
                             "unselectedGreenCardBtn"};
    gtk_widget_set_name(button, cardNames[c->type]);
    g_signal_connect (button, "clicked", G_CALLBACK(onSelectCard), c);

    //Introduce the button at the good place in the box
    GtkWidget *previousBtn = NULL, *btn = gtk_widget_get_first_child(GTK_WIDGET(cardBox));
    while (btn != NULL) {
        if (strlen(btnLabel) > strlen(gtk_button_get_label(GTK_BUTTON(btn))) ||
            (strcmp(btnLabel, gtk_button_get_label(GTK_BUTTON(btn))) > 0 &&
             strlen(btnLabel) == strlen(gtk_button_get_label(GTK_BUTTON(btn))))) {
            previousBtn = btn;
            btn = gtk_widget_get_next_sibling(GTK_WIDGET(btn));
        } else {
            btn = NULL;
        }
    }
    if (previousBtn == NULL) {
        gtk_box_prepend(GTK_BOX(cardBox), GTK_WIDGET(button));
    } else {
        gtk_box_insert_child_after(GTK_BOX(cardBox), GTK_WIDGET(button), GTK_WIDGET(previousBtn));
    }
}

void addCardLblForModify(Card *c, GtkWidget *checkBtn, GtkWidget *box) {
    GtkWidget *label, *newGrid;
    char labelNames[4][16] = {"modifGreyLabel", "modifBlueLabel", "modifRedLabel", "modifGreenLabel"};
    char labelLabel[12];
    sprintf(labelLabel, "Carte n°%d", c->id);
    label = gtk_label_new(labelLabel);
    gtk_widget_set_name(GTK_WIDGET(label), labelNames[c->type]);
    gtk_widget_set_name(GTK_WIDGET(checkBtn), "modifLabelCheck");
    newGrid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(newGrid), true);
    gtk_grid_attach(GTK_GRID(newGrid), GTK_WIDGET(label), 0, 0, 8, 1);
    gtk_grid_attach(GTK_GRID(newGrid), checkBtn, 7, 0, 1, 1);
    //Introduce the grid containing the label and the checkButton at the good place in the box
    GtkWidget *previousGrid = NULL, *oldGrid = gtk_widget_get_first_child(box);
    while (oldGrid != NULL) {
        if (strlen(labelLabel) >
            strlen(gtk_label_get_label(GTK_LABEL(gtk_widget_get_first_child(GTK_WIDGET(oldGrid))))) ||
            (strcmp(labelLabel, gtk_label_get_label(GTK_LABEL(gtk_widget_get_first_child(GTK_WIDGET(oldGrid))))) > 0 &&
             strlen(labelLabel) ==
             strlen(gtk_label_get_label(GTK_LABEL(gtk_widget_get_first_child(GTK_WIDGET(oldGrid))))))) {
            previousGrid = oldGrid;
            oldGrid = gtk_widget_get_next_sibling(GTK_WIDGET(oldGrid));
        } else {
            oldGrid = NULL;
        }
    }
    if (previousGrid == NULL) {
        gtk_box_prepend(GTK_BOX(box), GTK_WIDGET(newGrid));
    } else {
        gtk_box_insert_child_after(GTK_BOX(box), GTK_WIDGET(newGrid), GTK_WIDGET(previousGrid));
    }
}

void addLinkBtn(Link *l) {
    char btnLabel[9] = "";
    sprintf(btnLabel, "%d -> %d", l->parent->id, l->child->id);
    GtkWidget *button = gtk_button_new_with_label(btnLabel);
    char linkNames[4][23] = {"unselectedFoundLinkBtn", "unselectedCombLinkBtn", "unselectedFixedLinkBtn",
                             "unselectedHintLinkBtn"};
    gtk_widget_set_name(button, linkNames[l->type]);
    g_signal_connect (button, "clicked", G_CALLBACK(onSelectLink), l);

    //Introduce the button at the good place in the box
    GtkWidget *previousBtn = NULL, *btn = gtk_widget_get_first_child(GTK_WIDGET(linkBox));
    int newLinkParentId = l->parent->id, newLinkChildId = l->child->id, btnParentId, btnChildId;
    while (btn != NULL) {
        btnParentId = (int) strtol(gtk_button_get_label(GTK_BUTTON(btn)), NULL, 10);
        if (btnParentId > 9) btnChildId = (int) strtol(gtk_button_get_label(GTK_BUTTON(btn)) + 6, NULL, 10);
        else btnChildId = (int) strtol(gtk_button_get_label(GTK_BUTTON(btn)) + 5, NULL, 10);

        if (newLinkParentId > btnParentId || (newLinkParentId == btnParentId && newLinkChildId > btnChildId)) {
            previousBtn = btn;
            btn = gtk_widget_get_next_sibling(GTK_WIDGET(btn));
        } else {
            btn = NULL;
        }
    }
    if (previousBtn == NULL) {
        gtk_box_prepend(GTK_BOX(linkBox), GTK_WIDGET(button));
    } else {
        gtk_box_insert_child_after(GTK_BOX(linkBox), GTK_WIDGET(button), GTK_WIDGET(previousBtn));
    }
}

void removeLinkBtnFromCards(Card *parent, Card *child) {
    GtkWidget *tmp, *btn = gtk_widget_get_first_child(GTK_WIDGET(linkBox));
    char btnLabel[12] = "";
    sprintf(btnLabel, "%d -> %d", parent->id, child->id);
    while (btn != NULL) {
        tmp = gtk_widget_get_next_sibling(GTK_WIDGET(btn));
        if (strcmp(btnLabel, gtk_button_get_label(GTK_BUTTON(btn))) == 0) {
            gtk_box_remove(GTK_BOX(linkBox), btn);
        }
        btn = tmp;
    }
}

/* Window opening functions */

void openStartingWindow(GtkWindow *mainWindow) {
    if (curProject == NULL) {
        GtkBuilder *builder = gtk_builder_new();
        gtk_builder_add_from_file(builder, "../View/ViewBuilder.ui", NULL);
        GObject *startWindow, *newProjectBtn, *openProjectBtn, *closeStartingWindowBtn;
        startWindow = gtk_builder_get_object(builder, "startWindow");
        newProjectBtn = gtk_builder_get_object(builder, "newProjectBtn");
        openProjectBtn = gtk_builder_get_object(builder, "openProjectBtn");
        closeStartingWindowBtn = gtk_builder_get_object(builder, "closeStartingWindowBtn");
        gtk_window_set_default_size(GTK_WINDOW(startWindow), 300, 150);
        //Création du projet
        g_signal_connect_swapped(newProjectBtn, "clicked", G_CALLBACK(openNewProjectBrowser), mainWindow);

        g_signal_connect_swapped(newProjectBtn, "clicked", G_CALLBACK(gtk_window_close), startWindow);
        g_signal_connect_swapped(openProjectBtn, "clicked", G_CALLBACK(openOpenProjectBrowser), mainWindow);
        g_signal_connect_swapped(openProjectBtn, "clicked", G_CALLBACK(gtk_window_close), startWindow);
        g_signal_connect_swapped(closeStartingWindowBtn, "clicked", G_CALLBACK(g_application_quit),
                                 gtk_window_get_application(GTK_WINDOW(mainWindow)));
        gtk_widget_show(GTK_WIDGET (startWindow));
        g_object_unref(builder);
    } else {
        openMainWindow(mainWindow);
    }
}

void openNewProjectBrowser(GtkWidget *mainWindow) {
    /* Construct a GtkBuilder instance and load our UI description */
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "../View/ViewBuilder.ui", NULL);

    /* Connect signal handlers to the constructed widgets. */
    GObject *saveProjectWindow, *fileChooser, *abortBtn, *confirmBtn, *nameEntry;
    saveProjectWindow = gtk_builder_get_object(builder, "saveNewProjectWindow");
    g_signal_connect_swapped(saveProjectWindow, "destroy", G_CALLBACK(openStartingWindow), mainWindow);

    fileChooser = gtk_builder_get_object(builder, "saveNewProjectBrowser");
    char path[MAXPATH] = "/home/";
    strcat(path, getlogin());
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(fileChooser), g_file_new_for_path(path), NULL);

    nameEntry = gtk_builder_get_object(builder, "saveNewProjectEntry");
    gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(nameEntry)), "SansTitre", 9);

    abortBtn = gtk_builder_get_object(builder, "saveNewProjectAbort");
    g_signal_connect_swapped(abortBtn, "clicked", G_CALLBACK(gtk_window_close), saveProjectWindow);

    confirmBtn = gtk_builder_get_object(builder, "saveNewProjectConfirm");
    g_signal_connect_swapped(confirmBtn, "clicked", G_CALLBACK(onConfirmNewProject), saveProjectWindow);

    gtk_window_set_default_size(GTK_WINDOW(saveProjectWindow), 800, 500);

    gtk_widget_show(GTK_WIDGET (saveProjectWindow));
    g_object_unref(builder);
}

void openOpenProjectBrowser(GtkWidget *mainWindow) {
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "../View/ViewBuilder.ui", NULL);

    GObject *openProjectWindow, *abortBtn, *confirmBtn;
    openProjectWindow = gtk_builder_get_object(builder, "openProjectWindow");
    g_signal_connect_swapped(openProjectWindow, "destroy", G_CALLBACK(openStartingWindow), mainWindow);

    abortBtn = gtk_builder_get_object(builder, "openProjectAbort");
    g_signal_connect_swapped(abortBtn, "clicked", G_CALLBACK(gtk_window_close), openProjectWindow);

    confirmBtn = gtk_builder_get_object(builder, "openProjectConfirm");
    g_signal_connect_swapped(confirmBtn, "clicked", G_CALLBACK(onConfirmOpenProject), openProjectWindow);

    gtk_window_set_default_size(GTK_WINDOW(openProjectWindow), 800, 500);

    gtk_widget_show(GTK_WIDGET (openProjectWindow));
    g_object_unref(builder);
}

void openMainWindow(GtkWindow *window) {
    char windowName[MAXNAME + 14] = "";
    strcpy(windowName, curProject->name);
    strcat(windowName, " - UnlockMaker");
    gtk_window_set_title(GTK_WINDOW(window), windowName);
    gtk_widget_show(GTK_WIDGET(window));
    gtk_window_maximize(GTK_WINDOW(window));
}

void openModifyCardWindow() {
    if (selectedCard != NULL) {
        if (secondWindowOpen != NULL) {
            gtk_window_close(GTK_WINDOW(secondWindowOpen));
        }
        GtkBuilder *builder = gtk_builder_new();
        GObject *colorList, *numberCheck, *numberEntry, *rootCheck, *parentsBox, *childrenBox, *closeWindowBtn, *importImageBtn, *viewImageBtn;
        gtk_builder_add_from_file(builder, "../View/ViewBuilder.ui", NULL);

        /* Window */
        char windowTitle[31];
        sprintf(windowTitle, "Modification de la carte n°%d", selectedCard->id);
        secondWindowOpen = gtk_builder_get_object(builder, "modifCardWindow");
        gtk_window_set_title(GTK_WINDOW(secondWindowOpen), windowTitle);
        g_signal_connect(secondWindowOpen, "destroy", G_CALLBACK(onDestroySecondWindow), NULL);
        gtk_window_set_default_size(GTK_WINDOW(secondWindowOpen), 350, 700);
        gtk_window_set_resizable(GTK_WINDOW(secondWindowOpen), false);

        /* Color modification */
        colorList = gtk_builder_get_object(builder, "modifColorDropDown");
        g_signal_connect(GTK_WIDGET(colorList), "notify", G_CALLBACK(onModifyCardType), colorList);
        gtk_drop_down_set_selected(GTK_DROP_DOWN(colorList), selectedCard->type);

        /* Number modification */
        numberCheck = gtk_builder_get_object(builder, "modifNumberCheck");
        numberEntry = gtk_builder_get_object(builder, "modifNumberEntry");
        gtk_check_button_set_active(GTK_CHECK_BUTTON(numberCheck), selectedCard->fixedNumber);
        if (selectedCard->fixedNumber == 1) {
            char numberFixed[3];
            sprintf(numberFixed, "%d", selectedCard->number);
            gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(numberEntry)), numberFixed, (int) strlen(numberFixed));
        }
        g_signal_connect(GTK_WIDGET(numberEntry), "changed", G_CALLBACK(onEnterCardFixedNumber), numberCheck);
        g_signal_connect(GTK_WIDGET(numberCheck), "toggled", G_CALLBACK(onToggleFixedNumberCheck), numberEntry);

        /* Root modification */
        rootCheck = gtk_builder_get_object(builder, "modifRootCheck");
        g_signal_connect(GTK_WIDGET(rootCheck), "toggled", G_CALLBACK(onToggleSetCardAsRoot), NULL);
        gtk_check_button_set_active(GTK_CHECK_BUTTON(rootCheck), curProject->root == selectedCard);

        /* Image modification */
        importImageBtn = gtk_builder_get_object(builder, "modifCardAddImageBtn");
        viewImageBtn = gtk_builder_get_object(builder, "modifCardViewImageBtn");
        g_signal_connect_swapped(importImageBtn, "clicked", G_CALLBACK(openImportImageBrowser), "Card");
        g_signal_connect_swapped(viewImageBtn, "clicked", G_CALLBACK(openImageViewer), "Card");

        /* Parents/Children modification */
        parentsBox = gtk_builder_get_object(builder, "modifParentsBox");
        childrenBox = gtk_builder_get_object(builder, "modifChildrenBox");
        retrieveParentsChildren(GTK_BOX(parentsBox), GTK_BOX(childrenBox));

        /* Window closing */
        closeWindowBtn = gtk_builder_get_object(builder, "closeModifCardWindowBtn");
        g_signal_connect_swapped(closeWindowBtn, "clicked", G_CALLBACK(gtk_window_close), secondWindowOpen);

        gtk_widget_show(GTK_WIDGET (secondWindowOpen));
        g_object_unref(builder);
        changeInfoLabel("");
    }
}

void openModifyLinkWindow() {
    if (selectedLink != NULL) {
        if (secondWindowOpen != NULL) {
            gtk_window_close(GTK_WINDOW(secondWindowOpen));
        }
        GtkBuilder *builder = gtk_builder_new();
        GObject *typeList, *closeWindowBtn;
        gtk_builder_add_from_file(builder, "../View/ViewBuilder.ui", NULL);

        /* Window */
        char windowTitle[32];
        sprintf(windowTitle, "Modification du lien %d -> %d", selectedLink->parent->id, selectedLink->child->id);
        secondWindowOpen = gtk_builder_get_object(builder, "modifLinkWindow");
        gtk_window_set_title(GTK_WINDOW(secondWindowOpen), windowTitle);
        gtk_window_set_default_size(GTK_WINDOW(secondWindowOpen), 350, 100);
        gtk_window_set_resizable(GTK_WINDOW(secondWindowOpen), false);
        g_signal_connect(secondWindowOpen, "destroy", G_CALLBACK(onDestroySecondWindow), NULL);

        /* Type choice */
        typeList = gtk_builder_get_object(builder, "modifTypeDropDown");
        gtk_drop_down_set_selected(GTK_DROP_DOWN(typeList), selectedLink->type);
        g_signal_connect(GTK_WIDGET(typeList), "notify", G_CALLBACK(onModifyLinkType), typeList);

        /* Window closing */
        closeWindowBtn = gtk_builder_get_object(builder, "closeModifLinkWindowBtn");
        g_signal_connect_swapped(closeWindowBtn, "clicked", G_CALLBACK(gtk_window_close), secondWindowOpen);

        gtk_widget_show(GTK_WIDGET (secondWindowOpen));
        g_object_unref(builder);
        changeInfoLabel("");
    }
}

void openModifyProjectWindow() {
    if (secondWindowOpen != NULL) {
        gtk_window_close(GTK_WINDOW(secondWindowOpen));
    }
    GtkBuilder *builder = gtk_builder_new();
    GObject *closeWindowBtn, *importImageBtn, *viewImageBtn;
    gtk_builder_add_from_file(builder, "../View/ViewBuilder.ui", NULL);

    /* Window */
    char windowTitle[] = "Ajout d'images génériques au projet";
    secondWindowOpen = gtk_builder_get_object(builder, "modifProjectWindow");
    gtk_window_set_title(GTK_WINDOW(secondWindowOpen), windowTitle);
    g_signal_connect(secondWindowOpen, "destroy", G_CALLBACK(onDestroySecondWindow), NULL);
    gtk_window_set_default_size(GTK_WINDOW(secondWindowOpen), 400, 100);
    gtk_window_set_resizable(GTK_WINDOW(secondWindowOpen), false);

    /* Top image */
    importImageBtn = gtk_builder_get_object(builder, "addTopImageBtn");
    viewImageBtn = gtk_builder_get_object(builder, "viewTopImageBtn");
    g_signal_connect_swapped(importImageBtn, "clicked", G_CALLBACK(openImportImageBrowser), "Top");
    g_signal_connect_swapped(viewImageBtn, "clicked", G_CALLBACK(openImageViewer), "Top");

    /* Bottom image */
    importImageBtn = gtk_builder_get_object(builder, "addBottomImageBtn");
    viewImageBtn = gtk_builder_get_object(builder, "viewBottomImageBtn");
    g_signal_connect_swapped(importImageBtn, "clicked", G_CALLBACK(openImportImageBrowser), "Bottom");
    g_signal_connect_swapped(viewImageBtn, "clicked", G_CALLBACK(openImageViewer), "Bottom");

    /* Back image */
    importImageBtn = gtk_builder_get_object(builder, "addBackImageBtn");
    viewImageBtn = gtk_builder_get_object(builder, "viewBackImageBtn");
    g_signal_connect_swapped(importImageBtn, "clicked", G_CALLBACK(openImportImageBrowser), "Back");
    g_signal_connect_swapped(viewImageBtn, "clicked", G_CALLBACK(openImageViewer), "Back");

    /* Window closing */
    closeWindowBtn = gtk_builder_get_object(builder, "closeModifProjectWindowBtn");
    g_signal_connect_swapped(closeWindowBtn, "clicked", G_CALLBACK(gtk_window_close), secondWindowOpen);

    gtk_widget_show(GTK_WIDGET (secondWindowOpen));
    g_object_unref(builder);
    changeInfoLabel("");
}

void onDestroySecondWindow() {
    secondWindowOpen = NULL;
    if (imageViewerWindow != NULL) {
        gtk_window_close(GTK_WINDOW(imageViewerWindow));
    }
    changeInfoLabel("");
}

void openImportImageBrowser(char* target) {
    if(imageViewerWindow != NULL) {
        gtk_window_close(GTK_WINDOW(imageViewerWindow));
    }
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "../View/ViewBuilder.ui", NULL);

    GObject *importImageBrowser, *abortBtn, *confirmBtn;
    importImageBrowser = gtk_builder_get_object(builder, "importImageWindow");
    gtk_window_set_default_size(GTK_WINDOW(importImageBrowser), 800, 500);

    abortBtn = gtk_builder_get_object(builder, "importImageAbort");
    confirmBtn = gtk_builder_get_object(builder, "importImageConfirm");

    g_signal_connect_swapped(abortBtn, "clicked", G_CALLBACK(gtk_window_close), importImageBrowser);

    if(strcmp(target, "Card") == 0) {
        g_signal_connect_swapped(confirmBtn, "clicked", G_CALLBACK(onConfirmImportCardImage), importImageBrowser);
        gtk_window_set_title(GTK_WINDOW(importImageBrowser), "Importer une image pour la carte sélectionnée");
    }
    if(strcmp(target, "Top") == 0) {
        g_signal_connect_swapped(confirmBtn, "clicked", G_CALLBACK(onConfirmImportTopImage), importImageBrowser);
        gtk_window_set_title(GTK_WINDOW(importImageBrowser), "Importer une image de haut de carte");
    }
    if(strcmp(target, "Bottom") == 0) {
        g_signal_connect_swapped(confirmBtn, "clicked", G_CALLBACK(onConfirmImportBottomImage), importImageBrowser);
        gtk_window_set_title(GTK_WINDOW(importImageBrowser), "Importer une image de bas de carte");
    }
    if(strcmp(target, "Back") == 0) {
        g_signal_connect_swapped(confirmBtn, "clicked", G_CALLBACK(onConfirmImportBackImage), importImageBrowser);
        gtk_window_set_title(GTK_WINDOW(importImageBrowser), "Importer une image de dos de carte");
    }

    gtk_widget_show(GTK_WIDGET (importImageBrowser));
    g_object_unref(builder);
}

void openImageViewer(char* target) {
    if(imageViewerWindow != NULL) {
        gtk_window_close(GTK_WINDOW(imageViewerWindow));
    }
    int cardImage = 0;
    if (selectedCard != NULL) {
        cardImage = selectedCard->image;
    }
    if((cardImage && strcmp(target, "Card") == 0) || (curProject->topImage && strcmp(target, "Top") == 0) || (curProject->bottomImage && strcmp(target, "Bottom") == 0 ) || (curProject->backImage && strcmp(target, "Back") == 0)) {
        GtkBuilder *builder = gtk_builder_new();
        gtk_builder_add_from_file(builder, "../View/ViewBuilder.ui", NULL);

        GObject *imageViewer;
        imageViewerWindow = gtk_builder_get_object(builder, "imageViewerWindow");
        imageViewer = gtk_builder_get_object(builder, "imageViewer");
        char path[MAXPATH+MAXNAME+25] = "";

        if(strcmp(target, "Card") == 0) {
            sprintf(path, "%s/%s/Cards/Card%d/Image.jpg", curProject->path, curProject->name, selectedCard->id);
            gtk_window_set_title(GTK_WINDOW(imageViewerWindow), "Image de la carte sélectionnée");

        }
        if(strcmp(target, "Top") == 0) {
            sprintf(path, "%s/%s/Project/TopImage.jpg", curProject->path, curProject->name);
            gtk_window_set_title(GTK_WINDOW(imageViewerWindow), "Image de haut de carte");
        }
        if(strcmp(target, "Bottom") == 0) {
            sprintf(path, "%s/%s/Project/BottomImage.jpg", curProject->path, curProject->name);
            gtk_window_set_title(GTK_WINDOW(imageViewerWindow), "Image de bas de carte");
        }
        if(strcmp(target, "Back") == 0) {
            sprintf(path, "%s/%s/Project/BackImage.jpg", curProject->path, curProject->name);
            gtk_window_set_title(GTK_WINDOW(imageViewerWindow), "Image de dos de carte");
        }

        gtk_image_set_from_file(GTK_IMAGE(imageViewer), path);
        g_signal_connect(imageViewerWindow, "destroy", G_CALLBACK(onDestroyImageViewer), NULL);
        gtk_widget_show(GTK_WIDGET (imageViewerWindow));
        g_object_unref(builder);

        changeInfoLabel(path);
    } else {
        if(strcmp(target, "Card") == 0) {
            changeInfoLabel("La carte sélectionnée n'a pas d'image associée pour le moment.");
        }
        if(strcmp(target, "Top") == 0) {
            changeInfoLabel("Le projet n'a pas d'image de haut de carte associée pour le moment.");
        }
        if(strcmp(target, "Bottom") == 0) {
            changeInfoLabel("Le projet n'a pas d'image de bas de carte associée pour le moment.");
        }
        if(strcmp(target, "Back") == 0) {
            changeInfoLabel("Le projet n'a pas d'image de dos de carte associée pour le moment.");
        }
    }
}

void onDestroyImageViewer() {
    imageViewerWindow = NULL;
    changeInfoLabel("");
}

int openCloseConfirmationWindow(GtkWindow *mainWindow) {
    if (unsavedChanges == true) {
        GtkBuilder *builder = gtk_builder_new();
        gtk_builder_add_from_file(builder, "../View/ViewBuilder.ui", NULL);
        GObject *confirmWindow, *closeAbortBtn, *closeNoSaveBtn, *closeWithSaveBtn;
        confirmWindow = gtk_builder_get_object(builder, "closeConfirmWindow");
        gtk_window_set_default_size(GTK_WINDOW(confirmWindow), 300, 150);

        closeAbortBtn = gtk_builder_get_object(builder, "closeAbortBtn");
        g_signal_connect_swapped(closeAbortBtn, "clicked", G_CALLBACK(gtk_window_close), confirmWindow);

        closeNoSaveBtn = gtk_builder_get_object(builder, "closeNoSaveBtn");
        g_signal_connect(closeNoSaveBtn, "clicked", G_CALLBACK(onCloseNoSave), NULL);
        g_signal_connect_swapped(closeNoSaveBtn, "clicked", G_CALLBACK(g_application_quit),
                                 gtk_window_get_application(GTK_WINDOW(mainWindow)));

        closeWithSaveBtn = gtk_builder_get_object(builder, "closeWithSaveBtn");
        g_signal_connect_swapped(closeWithSaveBtn, "clicked", G_CALLBACK(saveProject), curProject);
        g_signal_connect(closeWithSaveBtn, "clicked", G_CALLBACK(deleteGraphFiles), NULL);
        g_signal_connect_swapped(closeWithSaveBtn, "clicked", G_CALLBACK(g_application_quit),
                                 gtk_window_get_application(GTK_WINDOW(mainWindow)));

        gtk_widget_show(GTK_WIDGET (confirmWindow));

        g_object_unref(builder);
        return true;
    }
    else {
        onCloseNoSave();
        return false;
    }
}

/* Widget managing functions */

void refreshCardBoxLabel() {
    char labelChar[25];
    if(curProject != NULL) {
        sprintf(labelChar, "Liste des cartes (%d/%d)", curProject->nbCards, MAXCARD);
    } else {
        sprintf(labelChar, "Liste des cartes (0/%d)", MAXCARD);
    }
    gtk_label_set_label(GTK_LABEL(cardBoxLabel), labelChar);
}
void refreshLinkBoxLabel() {
    char labelChar[24];
    if(curProject != NULL) {
        sprintf(labelChar, "Liste des liens (%d/%d)", curProject->nbLinks, MAXLINK);
    } else {
        sprintf(labelChar, "Liste des liens (0/%d)", MAXLINK);
    }
    gtk_label_set_label(GTK_LABEL(linkBoxLabel), labelChar);
}

void refreshSelectedCardLabel() {
    char text[28];
    if(selectedCard == NULL) {
        sprintf(text, "Aucune carte sélectionnée");
    } else {
        sprintf(text, "Carte sélectionnée : %d", selectedCard->id);
    }
    gtk_label_set_label(GTK_LABEL(selectedCardLabel), text);
    changeInfoLabel("");
}

void refreshSelectedLinkLabel() {
    char text[30];
    if(selectedLink == NULL) {
        sprintf(text, "Aucun lien sélectionnée");
    } else {
        sprintf(text, "Lien sélectionnée : %d -> %d", selectedLink->parent->id, selectedLink->child->id);
    }
    gtk_label_set_label(GTK_LABEL(selectedLinkLabel), text);
    changeInfoLabel("");
}

void changeInfoLabel(char *text) {
    gtk_label_set_label(GTK_LABEL(infoLabel), text);
}

void setCenterImagePath(char *path) {
    strcpy(imagePath, path);
    gtk_image_set_from_file(GTK_IMAGE(centerImage), imagePath);
}

void reloadCenterImage() {
    gtk_image_set_from_file(GTK_IMAGE(centerImage), imagePath);
}

void disableRightCardButtons() {
    gtk_widget_set_can_target(GTK_WIDGET(modifyCardBtn), false);
    gtk_widget_set_can_target(GTK_WIDGET(deleteCardBtn), false);
}

void disableRightLinkButtons() {
    gtk_widget_set_can_target(GTK_WIDGET(modifyLinkBtn), false);
    gtk_widget_set_can_target(GTK_WIDGET(deleteLinkBtn), false);
}

void enableRightCardButtons() {
    gtk_widget_set_can_target(GTK_WIDGET(modifyCardBtn), true);
    gtk_widget_set_can_target(GTK_WIDGET(deleteCardBtn), true);
}

void enableRightLinkButtons() {
    gtk_widget_set_can_target(GTK_WIDGET(modifyLinkBtn), true);
    gtk_widget_set_can_target(GTK_WIDGET(deleteLinkBtn), true);
}
