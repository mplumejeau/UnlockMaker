/*
// Created by vboxuser on 01/05/23.
*/
#include <stdio.h>

#include "../View/viewApplication.h"

#include "../Model/Project.h"
#include "../Model/VertexList.h"
#include "../Model/EdgeList.h"
#include "../Model/Card.h"
#include "../Model/Link.h"
#include "../Model/Struct.h"

#include "Controller.h"

Project * CurProject=NULL;

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

    CurProject = allocProject();
    /*Temporary projet path and name, to change depending on OS*/
    initProject(CurProject,"/home/vboxuser/Documents/Langage_C2/Projet/unlock-maker","NewProject");
}

void addNewCard(GtkApplication *app, gpointer user_data){
    addEmptyCard(CurProject);
}

/* Don't work for now*/
void closeProject(GtkApplication *app, gpointer user_data){
    if(CurProject!=NULL){
        deleteProject(CurProject);
    }
}

