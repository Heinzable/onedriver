#include <gtk/gtk.h>

/**
 * Creates a popup folder chooser via GTK.
 */
char *folder_chooser(char *title) {
    gtk_init(NULL, NULL);
    GtkFileChooserNative *dialog = gtk_file_chooser_native_new(
        title, NULL, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, NULL, NULL);
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_current_folder(chooser, g_get_home_dir());

    char *path = NULL;
    gint res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        path = gtk_file_chooser_get_filename(chooser);
    } else {
        path = strdup("");
    }
    gtk_native_dialog_destroy(GTK_NATIVE_DIALOG(dialog));
    g_object_unref(dialog);
    return path;
}
