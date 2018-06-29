#include <gtk/gtk.h>
#define GET_WIDGET(builder, x) GTK_WIDGET(gtk_builder_get_object(builder, x))


int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new_from_file("radiography.glade");
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "xraywindow"));
    gtk_builder_connect_signals(builder, NULL);
 
    g_object_unref(builder);
 
    gtk_widget_show(window);                
    gtk_main();
 
    return 0;
}
 
// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

void jump_to_address()
{
    GtkWidget* child;
    child = gtk_widget_new(GTK_TYPE_LABEL, "label", "Hello World", "xalign", 0.0, NULL);

    gtk_list_box_prepend((GtkListBox*)GET_WIDGET(builder, "address_list"), child);
}