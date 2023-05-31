
#include <gtk/gtk.h>
#include "../Model/Project.h"

#ifndef UNLOCKMAKER_VIEWAPPLICATION_H
#define UNLOCKMAKER_VIEWAPPLICATION_H

/* View initialization functions */

/**
 * Initializes all the view attached to the given app. This is the starting point
 * of the software after it is called by the main
 * @param app the app running during the time the software is open
 */
void initView(GtkApplication *app);

/**
 * Loads all the CSS style descriptions from the file ViewStyle.css
 */
void loadCss(void);

/* Widget adding/removing functions */

/**
 * Adds a button at the right place in the Card Box for the already created card,
 * and attach callbacks to it
 * @param c a pointer to the card
 */
void addCardBtn(Card* c);

/**
 * Adds a new label and the given checkButton at the right place in the given
 * Box in the card modification window
 * @param c a pointer to the card
 * @param checkBtn a pointer to the new checkButton related to the card
 * @param box a pointer to the box to which the widgets must be added
 */
void addCardLblForModify(Card* c, GtkWidget* checkBtn, GtkWidget* box);

/**
 * Adds a button at the right place in the Link Box for the already created link,
 * and attach callbacks to it
 * @param l a pointer to the link
 */
void addLinkBtn(Link* l);

/**
 * Remove all buttons in the Link Box that have the given parent and child cards
 * (there should only be one removed at max)
 * @param parent a pointer to the parent card
 * @param child a pointer to the child card
 */
void removeLinkBtnFromCards(Card* parent, Card* child);

/**
 * Change the color of all buttons in the Link Box that have the given parent and
 * child cards to match the new type (there should only be one changed at max)
 * @param parent a pointer to the parent card
 * @param child a pointer to the child card
 */
void changeLinkBtnTypeFromCards(Card *parent, Card *child, int newType);

/* Window opening functions */

/**
 * Opens the starting window that will allow to create or open a project
 * @param mainWindow a pointer to the main window of the software
 */
void openStartingWindow(GtkWindow *mainWindow);

/**
 * Opens the window that will allow to browse in the files, to create a new project
 * in the chosen directory
 * @param mainWindow a pointer to the main window of the software
 */
void openNewProjectBrowser(GtkWidget* mainWindow);

/**
 * Opens the window that will allow to browse in the files to open an already
 * created project
 * @param mainWindow a pointer to the main window of the software
 */
void openOpenProjectBrowser(GtkWidget* mainWindow);

/**
 * Opens the main window of the software and set its attributes
 * @param window a pointer to the main window
 */
void openMainWindow(GtkWindow *window);

/**
 * Opens the window that will allow to modify the attributes of the selected card
 */
void openModifyCardWindow();

/**
 * Opens the window that will allow to modify the attributes of the selected link
 */
void openModifyLinkWindow();

/**
 * Opens the window that will allow to add images to the project
 */
void openModifyProjectWindow();

/**
 * This function is called when either the modify Link or Card window is closed.
 * It changes the variable secondWindowOpen to NULL
 */
void onDestroySecondWindow();

/**
 * Opens the window that will allow to browse in the files to choose an image.
 * The image chosen will depend on the parameter 'cible' that will have the values
 * "card", "top", "bottom" and "back" corresponding to the image card and the
 * 3 generic images of the project
 * @param target the target of the browser
 */
void openImportImageBrowser(char* target);

/**
 * Opens the window that will allow to view an image. The image shown will depend
 * on the parameter 'target' that will have the values "card", "top", "bottom" and
 * "back" corresponding to the image card and the 3 generic images of the project
 * @param target the target of the viewer
 */
void openImageViewer(char* target);

/**
 * This function is called when the imageViewer is closed.
 * It changes the variable imageViewerWindow to NULL
 */
void onDestroyImageViewer();

/**
 * This function is called when the user asks to close the main window.
 * If there were unsaved changes, it opens the window that will ask for confirmation
 * before closing the window, and proposed to the user to save the project
 * @param mainWindow the main window of the software
 * @return true if the confirmation window is opened and the main window must not
 * be closed, false if all changes were saved and the main window can be closed
 */
int openCloseConfirmationWindow(GtkWindow *mainWindow);

/* Widget managing functions */

/**
 * Refreshes the count of the number of cards in the project on the card box
 */
void refreshCardBoxLabel();

/**
 * Refreshes the count of the number of links in the project on the link box
 */
void refreshLinkBoxLabel();

/**
 * Refreshes the bottom right label that indicates which card is selected
 */
void refreshSelectedCardLabel();

/**
 * Refreshes the bottom right label that indicates which link is selected
 */
void refreshSelectedLinkLabel();

/**
 * Changes the bottom right info label to display the given text
 * @param text the text to be displayed
 */
void changeInfoLabel(char* text);

/**
 * Sets the graph image widget to display the image at the given path
 * @param path the path of the image in the files
 */
void setCenterImagePath(char* path);

/**
 * Reloads the graph image widget to actualize the image. This function is called
 * when any modification to a link or a card is done
 */
void reloadCenterImage();

/**
 * Disables the card modification and suppression buttons to disallow any interaction
 * by the user. This function is called when the selected card is set to NULL
 */
void disableRightCardButtons();

/**
 * Disables the link modification and suppression buttons to disallow any interaction
 * by the user. This function is called when the selected link is set to NULL
 */
void disableRightLinkButtons();

/**
 * Disables the export button to disallow interactions by the user. This function is called
 * when a modification is done, so that the graph is not verified anymore
 */
void disableExportButton();

/**
 * Enables the card modification and suppression buttons to allow interactions
 * by the user. This function is called when the selected card is set to a non-NULL value
 */
void enableRightCardButtons();

/**
 * Enables the link modification and suppression buttons to allow interactions
 * by the user. This function is called when the selected link is set to a non-NULL value
 */
void enableRightLinkButtons();

/**
 * Enables the export button to allow interactions by the user. This function is called
 * when the graph is verified to be correct
 */
void enableExportButton();

#endif //UNLOCKMAKER_VIEWAPPLICATION_H
