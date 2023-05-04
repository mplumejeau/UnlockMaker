/*
// Created by vboxuser on 01/05/23.
*/

#ifndef UNLOCKMAKER_CONTROLLER_H
#define UNLOCKMAKER_CONTROLLER_H

void createNewProject_cb(GtkApplication *app, gpointer user_data);

void initGraphFile();
void updateGraphFile();

/**
 * Takes a text file in the format of graphviz and exports it on a png format
 * @param graphInput The input file containing the text graph data
 * @param PNGoutput The output file containing the PNG graph image
 * @return 1 if the function was successful, 0 otherwise
 */
int exportPNGGraphFromFile(char* dataInput, char* PNGOutput);

void onPressAddCard_cb(GtkWidget *widget, gpointer data);
void onSelectCard_cb(GtkWidget *widget, gpointer data);
void onPressDeleteCard_cb(GtkWidget *widget);
void onPressModifyCard_cb(GtkWidget *widget, gpointer data);

void closeProject(GtkApplication *app, gpointer user_data);

#endif /*UNLOCKMAKER_CONTROLLER_H*/
