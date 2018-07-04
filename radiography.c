#include <gtk/gtk.h>
#define GET_WIDGET(builder, x) GTK_WIDGET(gtk_builder_get_object(builder, x))

void jump_to_address(GtkWidget* button, gpointer user_data);

int main(int argc, char *argv[])
{
	GtkBuilder*                 builder;
	GtkWidget*                  window;
    GtkWidget*                  jump_button;
    GtkWidget*                  address_list_view;
    GtkTreeViewColumn*          address_column;
    GtkTreeViewColumn*          value_column;
    GtkListStore*               address_list_store;
    GtkCellRenderer*            address_renderer;
    GtkCellRenderer*            value_renderer;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new_from_file("radiography.glade");

	window = GET_WIDGET(builder, "xraywindow");
    jump_button = GET_WIDGET(builder, "jump_button");
    address_list_view = GET_WIDGET(builder, "address_list_view");
    address_column = GET_WIDGET(builder, "address_column");
    value_column = GET_WIDGET(builder, "value_column");

    address_list_store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

    /* Connect signals with callback functions */
	gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(jump_button, "clicked", G_CALLBACK(jump_to_address), address_list_store);
    
    /* Connect the list view to the list model */
    gtk_tree_view_set_model(GTK_TREE_VIEW(address_list_view), GTK_TREE_MODEL(address_list_store));

    /* Create the cell renderers*/
    address_renderer = gtk_cell_renderer_text_new();
    value_renderer = gtk_cell_renderer_text_new();

    /* Connect the columns to their renderers */
    gtk_tree_view_column_pack_start(address_column, address_renderer, TRUE);
    gtk_tree_view_column_pack_start(value_column, value_renderer, TRUE);

    /* Connect renderers to the list store */
    gtk_tree_view_column_add_attribute(address_column, address_renderer, "text", 0);
    gtk_tree_view_column_add_attribute(value_column, value_renderer, "text", 1);

	g_object_unref(builder);
    g_object_unref(address_list_store);

	gtk_widget_show(window);
	gtk_main();

	return 0;
}

// called when window is closed
void on_window_main_destroy()
{
	gtk_main_quit();
}

void jump_to_address(GtkWidget* button, gpointer user_data)
{
    GtkListStore* store = (GtkListStore*)user_data;
    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                       0, "Test",
                       1, "Text",
                       -1);
}
