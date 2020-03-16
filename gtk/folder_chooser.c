#include <gtk/gtk.h>

/**
 * Creates a popup folder chooser via GTK.
 */
char *folder_chooser(char *title) {
    gtk_init(NULL, NULL);
    GtkWindow *window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        title, window, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "Cancel",
        GTK_RESPONSE_CANCEL, "Select", GTK_RESPONSE_ACCEPT, NULL);
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_current_folder(chooser, g_get_home_dir());

    char *path = NULL;
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        path = gtk_file_chooser_get_filename(chooser);
    } else {
        path = strdup("");
    }
    gtk_widget_destroy(GTK_WIDGET(window));
    gtk_widget_destroy(dialog);
    return path;
}
