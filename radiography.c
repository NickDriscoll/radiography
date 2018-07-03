#include <gtk/gtk.h>
#define GET_WIDGET(builder, x) GTK_WIDGET(gtk_builder_get_object(builder, x))


int main(int argc, char *argv[])
{
	GtkBuilder      *builder;
	GtkWidget       *window;
    GtkTreeModel* address_list_model;
    GtkListStore* address_list_store;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new_from_file("radiography.glade");

	window = GET_WIDGET(builder, "xraywindow");
    address_list_model = GET_WIDGET(builder, "address_list_model");

    address_list_store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);

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

void jump_to_address(GtkButton* button, gpointer user_data)
{
    GtkListStore* store = (GtkListStore*)user_data;
    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    
}
