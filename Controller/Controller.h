/*
// Created by vboxuser on 01/05/23.
*/

#ifndef UNLOCKMAKER_CONTROLLER_H
#define UNLOCKMAKER_CONTROLLER_H

void createNewProject_cb(GtkApplication *app, gpointer user_data);

void initGraphFile();

/**
 * Takes a text file in the format of graphviz and exports it on a png format
 * @param graphInput The input file containing the text graph data
 * @param PNGoutput The output file containing the PNG graph image
 * @return 1 if the function was successful, 0 otherwise
 */

void getGraphDataPath(char* string);

int refreshGraphPNG();
int exportPNGGraphFromFile(char* dataInput, char* PNGOutput);

void onAddCard_cb(GtkWidget *widget, gpointer data);
void onSelectCard_cb(GtkWidget *widget, gpointer data);
void onSelectLink_cb(GtkWidget *widget, gpointer data);
void onPressDeleteCard_cb(GtkWidget *widget);
void onPressDeleteLink_cb(GtkWidget *widget);
void onModifyCardType_cb(int newType);
void onModifyLinkType_cb(int newType);
void onToggleCardFixedNumber_cb(int number); // -1 if fixNumber disabled, the number otherwise
void onEnterCardFixedNumber_cb(GtkWidget *entry, gpointer checkButton);
void onToggleCardRoot_cb(GtkWidget *checkButton);

void retrieveParentsChildren(GtkWidget *gridParents, GtkWidget *gridChildren);

void onToggleAddParent_cb(GtkWidget *widget, gpointer data);
void onToggleAddChild_cb(GtkWidget *widget, gpointer data);

void closeProject(GtkApplication *app, gpointer user_data);

#endif /*UNLOCKMAKER_CONTROLLER_H*/
