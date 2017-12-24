#include <gtk/gtk.h>

//data la entry
static void button_clicked(GtkWidget *widget,gpointer data)
{
	g_print("%s\n",gtk_entry_get_text(GTK_ENTRY(data)));
}
 
int main(int argc, char *argv[])
{
    /*GtkBuilder      *builder; 
    GtkWidget       *window;
 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "window_main.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);
 
    g_object_unref(builder);
 
    gtk_widget_show(window);                
    gtk_main();
 
    return 0;*/

	gtk_init(&argc,&argv);
	GtkWidget *window,*entry,*button,*hbox;

	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window,"delete_event",G_CALLBACK(gtk_main_quit),NULL);
	
	entry  = gtk_entry_new();
	button = gtk_button_new_with_mnemonic("_Write text");
	g_signal_connect(button,"clicked",G_CALLBACK(button_clicked),entry);
	hbox=gtk_hbox_new(0,0);
	gtk_box_pack_start(GTK_BOX(hbox),entry,0,0,0);
	gtk_box_pack_start(GTK_BOX(hbox),button,0,0,0);

	gtk_container_add(GTK_CONTAINER(window),hbox);
	


	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
 
// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
