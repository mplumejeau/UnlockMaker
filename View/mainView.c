/* Created by louenn on 14/03/23. */

/* This main can't be built unless a specific CMakeLists line is commented (until a fix is found) */

#include "viewApplication.h"

int main(int argc, char* argv[]) {
    #ifdef GTK_SRCDIR
    g_chdir (GTK_SRCDIR);
    #endif

    int status;

    GtkApplication *app = gtk_application_new("org.app.example", 0);
    g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return status;
}


