#define _GNU_SOURCE
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>

#define GET_WIDGET(builder, x) GTK_WIDGET(gtk_builder_get_object(builder, x))
#define GET_WINDOW(builder ,x) GTK_WINDOW(gtk_builder_get_object(builder, x))
#define GET_ENTRY(builder, x) GTK_ENTRY(gtk_builder_get_object(builder, x))
#define GET_COLUMN(builder, x) GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, x))

#define BUFFER_SIZE 1024

void jump_to_address(GtkWidget* button, gpointer user_data);
void attach_to_process(GtkWidget* button, gpointer user_data);

typedef unsigned char byte;

typedef struct
{
	GtkWindow*	window;
	GtkEntry*	box;
	pid_t*		pid;
} attach_s;

typedef struct
{
	GtkListStore*	list_store;
	GtkEntry*	entry;
	pid_t*		pid;
} read_s;

int main(int argc, char *argv[])
{
	/*Actual program variables */
	pid_t target_pid;

	/* GUI variables */
	GtkBuilder*			builder;
	GtkWindow*			window;
	GtkWidget*			jump_button;
	GtkWidget*			pid_button;
	GtkWidget*			address_list_view;
	GtkEntry*			pid_entry;
	GtkEntry*			address_entry;
	GtkTreeViewColumn*		address_column;
	GtkTreeViewColumn*		value_column;
	GtkListStore*			address_list_store;
	GtkCellRenderer*		address_renderer;
	GtkCellRenderer*		value_renderer;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new_from_file("radiography.glade");

	window = GET_WINDOW(builder, "xraywindow");
	pid_entry = GET_ENTRY(builder, "pid_entry");
	address_entry = GET_ENTRY(builder, "address_entry");
	jump_button = GET_WIDGET(builder, "jump_button");
	pid_button = GET_WIDGET(builder, "pid_button");
	address_list_view = GET_WIDGET(builder, "address_list_view");
	address_column = GET_COLUMN(builder, "address_column");
	value_column = GET_COLUMN(builder, "value_column");

	address_list_store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

	/* Create callback args */
	attach_s* pid_struct = malloc(sizeof(attach_s));
	pid_struct->window = window;
	pid_struct->box = pid_entry;
	pid_struct->pid = &target_pid;

	read_s* read_struct = malloc(sizeof(read_s));
	read_struct->list_store = address_list_store;
	read_struct->entry = address_entry;
	read_struct->pid = &target_pid;

	/* Connect signals with callback functions */
	gtk_builder_connect_signals(builder, NULL);
	g_signal_connect(jump_button, "clicked", G_CALLBACK(jump_to_address), read_struct);
	g_signal_connect(pid_button, "clicked", G_CALLBACK(attach_to_process), pid_struct);

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

	gtk_widget_show(GTK_WIDGET(window));
	gtk_main();

	return 0;
}

// called when window is closed
void on_window_main_destroy()
{
	gtk_main_quit();
}

void on_pid_text_entry(GtkEntry *entry, gchar* string, gpointer user_data)
{
	if (string[strlen(string) - 1] < 48 || string[strlen(string) - 1] > 57)
		string[strlen(string) - 1] = '\0';
}

void attach_to_process(GtkWidget* button, gpointer user_data)
{
	attach_s* pid_struct = (attach_s*)user_data;
	FILE* f;
	char file_path[BUFFER_SIZE] = {0};
	char new_window_title[BUFFER_SIZE] = {0};
	char proc_name[BUFFER_SIZE] = {0};
	pid_t new_pid;

	new_pid = atoi(gtk_entry_get_text(pid_struct->box));

	/* Make file_path string */
	sprintf(file_path, "/proc/%i/comm", new_pid);


	f = fopen(file_path, "r");
	if (f == NULL)
	{
		gtk_window_set_title(pid_struct->window, "ERROR ATTACHING TO PROCESS");
		return;
	}

	fread(proc_name, BUFFER_SIZE, 1, f);
	fclose(f);

	/* Remove linebreak */
	proc_name[strlen(proc_name) - 1] = '\0';

	/* Set window title */
	sprintf(new_window_title, "Attached to: %s", proc_name);
	gtk_window_set_title(pid_struct->window, new_window_title);

	/* Store pid */
	*pid_struct->pid = new_pid;
}

void jump_to_address(GtkWidget* button, gpointer user_data)
{
	GtkListStore*		store;
	GtkTreeIter		iter;
	pid_t			target_pid;
	struct iovec* local_vec;
	struct iovec* remote_vec;

	read_s* args = user_data;
	store = args->list_store;
	target_pid = *args->pid;
	local_vec = malloc(sizeof(struct iovec));
	remote_vec = malloc(sizeof(struct iovec));

	/* Set up local vector */
	local_vec->iov_len = 100;
	local_vec->iov_base = malloc(local_vec->iov_len);

	/* Set up remote vector */
	remote_vec->iov_len = 100;
	remote_vec->iov_base = (void*)atoll(gtk_entry_get_text(args->entry));

	/* Read memory from process */
	process_vm_readv(target_pid, local_vec, 1, remote_vec, 1, 0);

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter,
	                   0, "Test",
	                   1, "Text",
	                   -1);

	free(local_vec->iov_base);
	free(local_vec);
	free(remote_vec);
}
