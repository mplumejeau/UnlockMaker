
#ifndef UNLOCKMAKER_CONTROLLER_H
#define UNLOCKMAKER_CONTROLLER_H

/* -------------------- Classic fonctions -------------------- */

/**
 * Unselects the selected card, disabling the corresponding right buttons and displaying
 * the information that no card is currently selected
 */
void unselectCard();

/**
 * Unselects the selected link, disabling the corresponding right buttons and displaying
 * the information that no link is currently selected
 */
void unselectLink();

/**
 * Sets the card number to n if n is equal to 0 or above, or unfixes the card's number if n is equal to -1
 * @param n the new number of the card, or -1 if the number must be unfixed
 */
void setCardFixNumber(int n);

/**
 * Retrieves all the existing cards of the current project (except the current card),
 * adds them in both boxes of the modification window in the form of a label and a checkButton
 * and toggles ON all the cards that have a link with the selected card in the appropriate box
 * @param parentsBox a pointer to the box containing all the parent links of the card
 * @param childrenBox a pointer to the box containing all the child links of the card
 */
void retrieveParentsChildren(GtkBox *parentsBox, GtkBox *childrenBox);

/**
 * Adds a link in the project between a given parent and child
 * @param parent a pointer to the parent of the new link
 * @param child  a pointer to the child of the new link
 */
void addLinkFromToggle(Card *parent, Card *child);

/**
 * Deletes a link in the project between a given parent and child
 * @param parent a pointer to the parent of the deleted link
 * @param child  a pointer to the child of the deleted link
 */
void deleteLinkFromToggle(Card *parent, Card *child);

/* -------------------- Callbacks -------------------- */

/* Project managing callbacks */

/**
 * This Callback is called when the Confirmation button of the new project window is pressed\n\n
 * If a directory is selected, it initializes the project in this directory and closes the
 * starting windows to let place to the main window
 * @param newProjectWindow a pointer to the new project window to close it
 */
void onConfirmNewProject(GtkWidget *newProjectWindow);

/**
 * This Callback is called when the Confirmation button of the open project window is pressed\n\n
 * If a directory is selected, it opens it in the browser. If a file with the correct format
 * is opened, it opens the chosen project and closes the starting windows to let place to the
 * main window
 * @param openProjectWindow a pointer to the open project window to close it
 */
void onConfirmOpenProject(GtkWidget *openProjectWindow);

/**
 * This Callback is called when a Save button is pressed\n\n
 * It saves the informations of the project in a .txt file in a supported format
 */
void onSaveProject();

/**
 * This Callback is called when the Close-Without-Save button of the closing window is pressed\n\n
 * It deletes the project's file if the project was never saved and stops the app
 */
void onCloseNoSave();

/**
 * This Callback is called when the Confirmation button of the Import-Top-Image window is pressed\n\n
 * If a directory is selected, it opens it in the browser ; if a file is selected, it calls
 * the setTopImage function to assign the selected image to the top of the cards in the current project,
 * then closes the window
 * @param importImageWindow a pointer to the Import-Top-Image window
 */
void onConfirmImportTopImage(GtkWidget *importImageWindow);

/**
 * This Callback is called when the Confirmation button of the Import-Bottom-Image window is pressed\n\n
 * If a directory is selected, it opens it in the browser ; if a file is selected, it calls
 * the setBottomImage function to assign the selected image to the bottom of the cards in the current project,
 * then closes the window
 * @param importImageWindow a pointer to the Import-Bottom-Image window
 */
void onConfirmImportBottomImage(GtkWidget *importImageWindow);

/**
 * This Callback is called when the Confirmation button of the Import-Back-Image window is pressed\n\n
 * If a directory is selected, it opens it in the browser ; if a file is selected, it calls
 * the setBackImage function to assign the selected image to the back of the cards in the current project,
 * then closes the window
 * @param importImageWindow a pointer to the Import-Back-Image window
 */
void onConfirmImportBackImage(GtkWidget *importImageWindow);

/**
 * This Callback is called when the Debug button is pressed\n\n
 * It saves the project, then calls the function checkLoops from the model to check the graph. If the
 * graph is valid, then it enables the export button
 */
void onCheckLoops();

/**
 * This Callback is called when the Export button is pressed\n\n
 * It calls the algorithm assignNumbers from the model and if it is successful, then calls createPrintable
 * who exports the project as a pdf file
 */
void onExport();

/* Card managing callbacks */

/**
 * This Callback is called when the Add-Card button is pressed\n\n
 * It calls the functions to add the card in the project, in the graph, and in the view
 */
void onAddCard();

/**
 * This Callback is called when a Card button is pressed\n\n
 * It sets the selected card to the card given in parameter and changes the appearance of
 * the button pressed
 * @param cardBtn a pointer to the new selected card's button
 * @param card a pointer to the new selected card
 */
void onSelectCard(GtkWidget *cardBtn, Card *card);

/**
 * This Callback is called when the Color dropdown in the card modification window has its selected color changed\n\n
 * It sets the selected card's color to the new selected color in the dropdown
 * @param dropDown a pointer to the color modification dropdown
 */
void onModifyCardType(GtkWidget *dropDown);

/**
 * This Callback is called when the Card-Fixed-Number checkButton in the card modification window is toggled\n\n
 * It calls the function setCardFixedNumber with the parameter -1 is the box is toggled OFF,
 * or with the number in the number entry if it is toggled ON (0 if the entry is empty).
 * It also disables the entry if the checkButton is toggled OFF
 * @param checkButton a pointer to the fixed-number checkButton
 * @param entry a pointer to the Fixed-Number entry
 */
void onToggleFixedNumberCheck(GtkWidget *checkButton, GtkWidget *entry);

/**
 * This Callback is called when the Card-Fixed-Number entry in the card modification window is being interacted with\n\n
 * If the fixed-number checkButton is toggled ON, it calls the function setCardFixedNumber
 * with the parameter equal to the number inside the entry
 * @param entry a pointer to the Fixed-Number entry
 * @param checkButton a pointer to the Fixed-Number checkButton
 */
void onEnterCardFixedNumber(GtkWidget *entry, GtkWidget *checkButton);

/**
 * This Callback is called when the Set-As-Root checkButton in the card modification window is toggled\n\n
 * It sets the root attribute of the current project to the selected card if the box was
 * toggled ON, or to NULL if the box was toggled OFF
 * @param checkButton a pointer to the Set-As-Root checkButton
 */
void onToggleSetCardAsRoot(GtkWidget *checkButton);

/**
 * This Callback is called when the Confirmation button of the Import-Card-Image window is pressed\n\n
 * If a directory is selected, it opens it in the browser ; if a file is selected, it calls
 * the setCardImage function to assign the selected image to the selected card in the current project,
 * then closes the window
 * @param importImageWindow a pointer to the Import-Card-Image window
 */
void onConfirmImportCardImage(GtkWidget *importImageWindow);

/**
 * This Callback is called when a parent checkButton of the modification window is toggled\n\n
 * If the box is toggled ON, it adds a link between the selected card and its parent with
 * the function addLinkFromToggle ; if the box is toggled OFF, it deletes the link with
 * the function deleteLinkFromToggle
 * @param parentCheckBtn a pointer to the parent checkButton toggled
 * @param parentCard a pointer to the card set or removed as parent of the selected card
 */
void onToggleAddParent(GtkWidget *parentCheckBtn, Card *parentCard);

/**
 * This Callback is called when a child checkButton of the modification window is toggled\n\n
 * If the box is toggled ON, it adds a link between the selected card and its child with
 * the function addLinkFromToggle ; if the box is toggled OFF, it deletes the link with
 * the function deleteLinkFromToggle
 * @param childCheckBtn a pointer to the card checkButton toggled
 * @param childCard a pointer to the card set or removed as child of the selected card
 */
void onToggleAddChild(GtkWidget *childCheckBtn, Card *childCard);

/**
 * This Callback is called when the Delete-Card button is pressed\n\n
 * It calls the functions to delete the card and all of its links in the project,
 * in the graph, and in the view
 */
void onPressDeleteCard();

/* Link managing callbacks */

/**
 * This Callback is called when a Link button is pressed\n\n
 * It sets the selected link to the link given in parameter and changes the appearance of
 * the button pressed
 * @param linkBtn a pointer to the new selected link's button
 * @param link a pointer to the new selected link
 */
void onSelectLink(GtkWidget *linkBtn, Link *link);

/**
 * This Callback is called when the Type dropdown in the link modification window has its selected type changed\n\n
 * It sets the selected link's type to the new selected type in the dropdown
 * @param dropDown a pointer to the type modification dropdown
 */
void onModifyLinkType(GtkWidget *dropDown);

/**
 * This Callback is called when the Delete-Link button is pressed\n\n
 * It calls the functions to delete the link in the project, in the graph, and in the view
 */
void onPressDeleteLink();

/* Image managing callbacks */

/**
 * This Callback is called when the Zoom scale button has its cursor moved\n\n
 * It changes the zoom of the center graph image in its window depending on the position
 * of the cursor of the scale button
 * @param zoomBtn a pointer to the zoom scale button
 * @param image a pointer to the graph image
 */
void onChangeImageZoom(GtkWidget *zoomBtn, gpointer image);

#endif /*UNLOCKMAKER_CONTROLLER_H*/
