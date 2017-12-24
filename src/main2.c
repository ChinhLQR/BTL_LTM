#include <stdio.h>
#include <stdlib.h>

#include <glib.h>

#include <gtk/gtk.h>


GtkBuilder      *builder; 

//Cua so login
GtkWidget       *entry_name;
GtkWidget       *entry_pass;
GtkWidget       *label;

//Cua so chat
GtkWidget       *view_chat;
GtkWidget       *entry_chat;
GtkTextIter iter;
GtkTextBuffer *buffer;

int main(int argc, char *argv[])
{
    
    GtkWidget       *window;
    
 
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "window_main.glade", NULL);
 
    //Cua so login
    window = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Login"));
    label = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Status"));
    entry_name = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_Name"));
    entry_pass = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_Pass"));

    //Cua so chat
    view_chat = GTK_WIDGET(gtk_builder_get_object(builder,"View_Chat"));
    entry_chat = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_Chat"));

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


//Xu li Login
void on_Btn_Login_clicked(GtkButton *button,GtkWindow *window_login)
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
    builder = gtk_builder_new();

if(g_utf8_collate (gtk_entry_get_text(entry_name),"hello")==0&&g_utf8_collate (gtk_entry_get_text(entry_pass),"bye")==0)
    {
    gtk_builder_add_from_file (builder, "window_main.glade", NULL); 
    gtk_widget_hide(window_login);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Chat"));
    //xam
    gtk_builder_connect_signals(builder, NULL);
    view_chat = GTK_WIDGET(gtk_builder_get_object(builder,"View_Chat"));
    entry_chat = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_Chat"));


    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view_chat));
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
    //g_object_unref(builder);
    //endxam
    gtk_widget_show(window);
    }
else
    {
    gtk_label_set_text(GTK_LABEL(label),"Login Failed , try again !");
    reset_name_pass();
    }
}

//Xu li SignUp
void on_Btn_SignUp_clicked()
{   
 
    if(gtk_entry_get_text_length(entry_name)<5)
    {
        gtk_label_set_text(GTK_LABEL(label),"User name too short,at least 5 characters");
        reset_name_pass();
    }
    else if(gtk_entry_get_text_length(entry_pass)<5)
    {
        gtk_label_set_text(GTK_LABEL(label),"Password too short,at least 5 characters");
        reset_name_pass();
    }
    else if(g_utf8_collate (gtk_entry_get_text(entry_name),"hello")==0 )
    {
        gtk_label_set_text(GTK_LABEL(label),"User name existed, please try again");
        reset_name_pass();
    }
    else
    gtk_label_set_text(GTK_LABEL(label),"Signed Up!");
}


void on_Btn_Send_clicked()
{

    //de day thi se bi ko xuong dong ma len nguoc, nen vut len tren khi vua chuyen window
   // GtkTextIter iter;
    //GtkTextBuffer *buffer;
   // buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view_chat));
    //gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);

    gtk_text_buffer_insert(buffer,&iter,"you: ",-1);
    gtk_text_buffer_insert(buffer,&iter,gtk_entry_get_text(entry_chat),-1);
    gtk_text_buffer_insert(buffer,&iter,"\n",-1);
    gtk_entry_set_text(entry_chat,"");
    



   gtk_text_view_set_buffer(view_chat,buffer);
}

void reset_name_pass()
{
    gtk_entry_set_text(entry_name,"");
    gtk_entry_set_text(entry_pass,"");
}

