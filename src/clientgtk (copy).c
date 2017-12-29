/// Tat cua so thi delete trong array
/// list online

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 3000
#define MESSAGE_LENGTH 300
#define MAXLINE 4096

GtkBuilder      *builder;

//Cua so login
GtkWidget       *window_login;
GtkWidget       *entry_name;
GtkWidget       *entry_pass;
GtkWidget       *label;

//Window chat

GtkWidget       *window_chat;

GtkWidget       *entry_friend;
GtkWidget	    *entry_add;
GtkWidget       *entry_del;
GtkWidget       *label_myname;

GtkWidget       *window_notice;
GtkWidget       *label_notice;

GtkWidget       *box_tree;
GtkWidget       *list;
const char *arrayOnline[20];
int arrayOnlineCount = 0

GtkTreeSelection *selection;

GtkWidget *Array_Chat_Name_Label[100];
int array_chat_name_count = 0;

int server = -1;
char username[20];
char friendname[20];

enum {
    
      LIST_ITEM = 0,
      N_COLUMNS
    };
    
void init_list(GtkWidget *list) {
    
      GtkCellRenderer *renderer;
      GtkTreeViewColumn *column;
      GtkListStore *store;
    
      renderer = gtk_cell_renderer_text_new ();
      column = gtk_tree_view_column_new_with_attributes("List Items",
              renderer, "text", LIST_ITEM, NULL);
      gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
    
      store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
    
      gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
          GTK_TREE_MODEL(store));
    
      g_object_unref(store);
}

void add_to_list(GtkWidget *list, const gchar *str) {
        
      GtkListStore *store;
      GtkTreeIter iter;
    
      store = GTK_LIST_STORE(gtk_tree_view_get_model
          (GTK_TREE_VIEW(list)));
    
      gtk_list_store_append(store, &iter);
      gtk_list_store_set(GTK_LIST_STORE (store), &iter, LIST_ITEM, str, -1);
}


void remove_item(GtkWidget *widget, const gchar *str) {
    
  GtkListStore *store;
  GtkTreeModel *model;
  GtkTreeIter  iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));

  if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) {
      return;
  }

  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), 
         &model, &iter)) {
    gtk_list_store_remove(store, &iter);
  }
}

void remove_all(GtkWidget *widget) {
    
  GtkListStore *store;
  GtkTreeModel *model;
  GtkTreeIter  iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));

  if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) {
      return;
  }
  
  gtk_list_store_clear(store);
}
    
void on_changed(GtkWidget *widget, gpointer label) {
        
      GtkTreeIter iter;
      GtkTreeModel *model;
      gchar *value;
    
      if (gtk_tree_selection_get_selected(
          GTK_TREE_SELECTION(widget), &model, &iter)) {
    
        gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
        
        gtk_entry_set_text(GTK_ENTRY(entry_friend),value);
        g_free(value);
      }
}



GtkWidget*   find_child(GtkWidget* parent, const gchar* name)
{
        if (g_strcasecmp(gtk_widget_get_name((GtkWidget*)parent), (gchar*)name) == 0) { 
              printf("~~~~~~~~~~tim thay~~~~~~~~~~");  
              return parent;
        }

        if (GTK_IS_BIN(parent)) {
                GtkWidget *child = gtk_bin_get_child(GTK_BIN(parent));
                return find_child(child, name);
        }

        if (GTK_IS_CONTAINER(parent)) {
                GList *children = gtk_container_get_children(GTK_CONTAINER(parent));
              //   while ((children = g_list_next(children)) != NULL)
              //   {
              //           GtkWidget* widget = find_child(children->data, name);
              //           if (widget != NULL) 
              //           {
              //                   return widget;
              //           }
              //   }
              do
              {
                  if(children!=NULL)
                  {
                      GtkWidget* widget = find_child(children->data, name);
                      if (widget != NULL) 
                      {
                              return widget;
                      }
                  }
              }
              while ((children = g_list_next(children)) != NULL);
        }

        return NULL;
}
void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}
int connect_server(char* server_ip, int port) {



    // 1 Tao socket
    struct sockaddr_in server_address;
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) 
    {
      printf("ERROR: socket()!\n");
      return 1;
    }
  
    // 2 Connect
    // 2.1 Tao server_address
    
    server_address.sin_family = AF_INET;
    // cach 2 su dung inet_addr. Method nay khong duoc khuyen khich
    //server_address.sin_addr.s_addr = inet_addr(server_ip);
    inet_aton(server_ip, &(server_address.sin_addr));
    server_address.sin_port = htons(port);
  
    // 2.2 Connect
    int connect_result = connect(server, (struct sockaddr*)&server_address, sizeof(server_address));
    if (connect_result < 0) 
    {
      printf("ERROR: connect()\n");
    }
    return server;
  }


void signio_handler(int signo){
    
    char  *recvline;
    char buff[1024];
    char* p;
    int count_friend=0;

    //GtkWidget *window_friend[100];

    int n = recv(server, buff, sizeof buff, 0);
    if (n>0)
    {
        
        buff[n]='\0';
        printf("Received from server (%d bytes), content: %s\n",n, buff);
        recvline = buff;
        p = strtok(recvline, "|");





        ////////////////////Tin hieu tu Sign up
        if (strcmp(p,"101")==0)
        {
               gtk_label_set_text(GTK_LABEL(label),"Sign Up Success");
        }
        if (strcmp(p,"102")==0)
        {
               gtk_label_set_text(GTK_LABEL(label),"Sign Up Failed");
        }
         ///////////////////End Tin hieu tu Sign up
    





        /////////////// Tin hieu tu login
        if (strcmp(p,"201")==0)
        {
            gtk_widget_hide(window_login);
            window_chat = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Chat"));
           /////~~~~~~~~~~~ko co' la` sap ~~~~~~~~~~~~~~~~/////
           ////xung dot voi show cua 402? ////////
            waitFor(1);
            gtk_widget_show(window_chat);
            

            entry_friend = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_Friend"));
            entry_add = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_AddFriend"));
            entry_del = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_DeleteFriend"));
            label_myname = GTK_WIDGET(gtk_builder_get_object(builder,"Label_MyName"));
        
            
            //Phan cua treeview
             

          
            selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
          
            g_signal_connect(selection, "changed", 
            G_CALLBACK(on_changed), label);
          
          
            gtk_widget_show_all(window_chat);
            //~~~~~~~~~~~~~~~
          


            
            char welcome[60] = "WelCome ";
            strcat(welcome,username);
            
            gtk_label_set_text(GTK_LABEL(label_myname),welcome);
           
        }

        if (strcmp(p,"202")==0){
            GtkTextIter iter;
            GtkTextBuffer *buffer;
            p = strtok(NULL, "|");
            char *friend_oo = p;
            p = strtok(NULL,"|");
            char *status = p;
            if(strcmp(status,"1")==0)
            {
            add_to_list(list,friend_oo);
                arrayOnline[arrayOnlineCount] = friend_oo;
                arrayOnlineCount++;
            }
            

            gtk_widget_show_all(window_chat);
        }

        if (strcmp(p,"203")==0){}

        if (strcmp(p,"204")==0){            
            GtkTextIter iter;
            GtkTextBuffer *buffer;
            p = strtok(NULL, "|");
            char *friend_oo = p;
            add_to_list(list,friend_oo);
            arrayOnline[arrayOnlineCount] = friend_oo;
            arrayOnlineCount++;
            gtk_widget_show_all(window_chat);
        }
        


        if (strcmp(p,"205")==0)
        {
        gtk_label_set_text(GTK_LABEL(label),"Login Failed , try again !");
        reset_name_pass();
        printf("login error\n");
        
        }
        /////////////////// End Tin hieu tu login




        ////////////////////Tin hieu tu Dang Xuat
        if (strcmp(p,"301")==0){}
        if (strcmp(p,"302")==0){
            GtkTextIter iter;
            GtkTextBuffer *buffer;
            p = strtok(NULL, "|");
            char *friend_oo = p;
            
            for(int i=0;i<arrayOnlineCount;i++)
            {
               
                if(strcmp(friend_oo,gtk_label_get_text(arrayOnline[i]))==0)
                { 
                    arrayOnline[i] = arrayOnline[arrayOnlineCount-1];
                    arrayOnline[arrayOnlineCount-1]=NULL;
                    arrayOnlineCount--;
                }
            }
            for(int i=0;i<arrayOnlineCount;i++)
            {
            add_to_list(list,arrayOnline[i]);
            }
        }
        if (strcmp(p,"303")==0){}
        //////////////////// End Tin hieu Dang Xuat



        ////////////////TIn hieu tu yeu cau Ket Ban
        if (strcmp(p,"401")==0||strcmp(p,"403")==0){
            window_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Notice"));    
            gtk_widget_show(window_notice);
            label_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Notice"));
            p = strtok(NULL, "|");
            gtk_label_set_text(GTK_LABEL(label_notice),p);
        }
        if (strcmp(p,"402")==0){
            count_friend++;
            GtkWidget *window_friend = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Friend"));
            GtkWidget *label_friend = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Friend"));
            GtkWidget *label_friend_pointer =GTK_WIDGET(gtk_builder_get_object(builder, "Label_Friend_Pointer"));
            p = strtok(NULL, "|"); 
            gtk_label_set_text(GTK_LABEL(label_friend_pointer),p);
            gtk_widget_show(window_friend);
            char dialog[60] = "Friend Request from ";
            strcat(dialog,p);
            gtk_label_set_text(GTK_LABEL(label_friend),dialog);
        }
        //////////////////End tin hieu Yeu cau Ket Ban

        ///////////////////Tin hieu tu chap nhan ket ban
        //501: gui thanh cong/ 502:
        if (strcmp(p,"501")==0||strcmp(p,"502")==0 || strcmp(p,"503")==0 ){
            window_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Notice"));    
            gtk_widget_show(window_notice);
            label_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Notice"));
            p = strtok(NULL, "|");
            gtk_label_set_text(GTK_LABEL(label_notice),p);
        }
        /////////////////////End tin hieu chap nhan ket ban

      

        ///////////////////Tin hieu tu Tu Choi Ket Ban // TIn hieu tu xoa ban
        if (strcmp(p,"601")==0||strcmp(p,"602")==0||strcmp(p,"701")==0||strcmp(p,"702")==0||strcmp(p,"703")==0||strcmp(p,"803")==0){
            window_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Notice"));    
            gtk_widget_show(window_notice);
            label_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Notice"));
            p = strtok(NULL, "|");
            gtk_label_set_text(GTK_LABEL(label_notice),p);
        }
      
        ///////////////////End tin hieu Tu Choi Ket Ban // Tin hieu tu xoa ban


          
        
        
        
        
        //////////////////Tin hieu tu Gui Tin Nhan
        if (strcmp(p,"802")==0){
            GtkTextIter iter;
            GtkTextBuffer *buffer;
            int opened=0;
            int save;
            p = strtok(NULL, "|");
            char *friend = p;
            p = strtok(NULL,"|");
            char *mess = p;

            for(int i=0;i<array_chat_name_count;i++)
            {
                if(array_chat_name_count!=0)
                {
                if(strcmp(friend,gtk_label_get_text(Array_Chat_Name_Label[i]))==0)
                {opened = 1;
                 save =i;
                }
                }
            }

            if(opened==1)
            {
            GtkWidget* view_chat = find_child(gtk_widget_get_toplevel (Array_Chat_Name_Label[save]),"GtkTextView");
            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view_chat));
            gtk_text_buffer_get_iter_at_offset(buffer, &iter, -1);
            gtk_text_buffer_insert(buffer,&iter,friend,-1);
            gtk_text_buffer_insert(buffer,&iter,": ",-1);
            gtk_text_buffer_insert(buffer,&iter,mess,-1);
            gtk_text_buffer_insert(buffer,&iter,"\n",-1);
            }
            else
            {
                
                gtk_builder_add_from_file (builder, "window_main.glade", NULL); 
                GtkWidget *window_chat_mini = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Chat_Mini"));
                gtk_builder_connect_signals(builder, NULL);
                gtk_widget_show(window_chat_mini);
                Array_Chat_Name_Label[array_chat_name_count] = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Chat_Name"));
        
                gtk_label_set_text(GTK_LABEL(Array_Chat_Name_Label[array_chat_name_count]),friend);
                GtkWidget* view_chat = find_child(gtk_widget_get_toplevel (Array_Chat_Name_Label[array_chat_name_count]),"GtkTextView");
                buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view_chat));
                gtk_text_buffer_get_iter_at_offset(buffer, &iter, -1);
                gtk_text_buffer_insert(buffer,&iter,friend,-1);
                gtk_text_buffer_insert(buffer,&iter,": ",-1);
                gtk_text_buffer_insert(buffer,&iter,mess,-1);
                gtk_text_buffer_insert(buffer,&iter,"\n",-1);
                array_chat_name_count++;
                
            }
            


        }

        //////////////////End tin hieu tu Gui Tin Nhan

    }
}















int main(int argc, char *argv[])
  {  
      //connect server
      server = connect_server("127.0.0.1",8484);

      //signal
      if(fcntl(server, F_SETFL, O_NONBLOCK|O_ASYNC))
      printf("Error in setting socket to async, nonblock mode");  
   
      signal(SIGIO, signio_handler); 
   
      if (fcntl(server,F_SETOWN, getpid()) <0)
      printf("Error in setting own to socket");

      gtk_init(&argc,&argv);
      builder = gtk_builder_new();
      gtk_builder_add_from_file(builder,"window_main.glade",NULL);

      window_login = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Login"));
      label = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Status"));
      entry_name = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_Name"));
      entry_pass = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_Pass"));
      box_tree = GTK_WIDGET(gtk_builder_get_object(builder,"box3"));






      list = gtk_tree_view_new();
      gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
      gtk_box_pack_start(GTK_BOX(box_tree), list, TRUE, TRUE, 5);
      gtk_box_pack_start(GTK_BOX(box_tree), label, FALSE, FALSE, 5);
    
      init_list(list);
      add_to_list(list,"loz");






      gtk_builder_connect_signals(builder, NULL);
      //g_object_unref(builder);
      gtk_widget_show(window_login);
      gtk_main();
      return 0;
  }



  void on_window_main_destroy()
  {   
      
    char sendline[MAXLINE];
    char head[60]="LGOT|";		
    
    strcat(head,username);
    strcat(head,"\0");
    send(server, head, strlen(head), 0);
      
    strcpy(username,"");
    gtk_main_quit();
      
  }

  void on_Btn_Login_clicked()
  {
      char user[20],pass[20];
      char head[60]="LGIN|";
      char sendline[MAXLINE];

      strcat(username,gtk_entry_get_text(entry_name));
      strcat(head,gtk_entry_get_text(entry_name));
      strcat(head,"|");
      strcat(head,gtk_entry_get_text(entry_pass));
      strcat(head,"\0");
      strcpy(sendline,head);
      send(server, sendline, strlen(sendline), 0);
  }

  void on_Btn_SignUp_clicked()
  {   
      char user[20],pass[20];
      char head[60]="SGNU|";
      char sendline[MAXLINE];

      strcat(head,gtk_entry_get_text(entry_name));
      strcat(head,"|");
      strcat(head,gtk_entry_get_text(entry_pass));
      strcat(head,"\0");
      strcpy(sendline,head);
      send(server, sendline, strlen(sendline), 0);
      printf("%s\n",sendline);
  
  }

  void on_Btn_AddFriend_clicked()
  {
      char head[60]="FREQ|";
      char sendline[MAXLINE]; 

      strcat(head,gtk_entry_get_text(entry_add));
      strcat(head,"\0");
      strcpy(sendline,head);
      send(server, sendline, strlen(sendline), 0);
      printf("%s\n",sendline);
  }

  void on_Btn_DeleteFriend_clicked()
  {  
    char head[60]="DELF|";
    char sendline[MAXLINE]; 
    
    strcat(head,gtk_entry_get_text(entry_add));    
    strcat(head,"\0");
    strcpy(sendline,head);
    send(server, sendline, strlen(sendline), 0);
    printf("%s\n",sendline);  
  }


  void on_Btn_Notice_clicked()
  {
    gtk_widget_hide(window_notice);
  }

  void on_Btn_Accept_clicked(GtkButton *button, GtkLabel *friend_label)
  {
    char head[60]="ACCR|";
    char* p;
    char sendline[MAXLINE];
  
    strcat(head,gtk_label_get_text(friend_label));
    strcat(head,"\0");
    strcpy(sendline,head);
    send(server, sendline, strlen(sendline), 0);
    printf("%s\n",sendline);
    strcpy(friendname,"");
    GtkWidget *window_friend = gtk_widget_get_toplevel (friend_label);
    gtk_widget_hide(window_friend);
  }

  void on_Btn_Refuse_clicked(GtkButton *button, GtkLabel *friend_label)
  {
    char head[60]="DECR|";
    char* p;
    char sendline[MAXLINE];
  
    strcat(head,gtk_label_get_text(friend_label));
    strcat(head,"\0");
    strcpy(sendline,head);
    send(server, sendline, strlen(sendline), 0);
    printf("%s\n",sendline);
    strcpy(friendname,"");
    GtkWidget *window_friend = gtk_widget_get_toplevel (friend_label);
    gtk_widget_hide(window_friend);
  }

  
  void on_Btn_Chat_clicked()
  {
        //Kiem tra ton tai
        int duplicate = 0;
        // chu y phai nho hon ko phai <=
        for(int i=0;i<array_chat_name_count;i++)
        {
            if(array_chat_name_count!=0)
            {
            if(strcmp(gtk_entry_get_text(entry_friend),gtk_label_get_text(Array_Chat_Name_Label[i]))==0)
            duplicate = 1;
            }
        }
        if(duplicate==0) printf("ko trung");
        else printf("co trung");
        if(duplicate == 0)
        {
        char friend_name[60];
        strcpy(friendname,gtk_entry_get_text(entry_friend));
        gtk_builder_add_from_file (builder, "window_main.glade", NULL); 
        GtkWidget *window_chat_mini = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Chat_Mini"));
        gtk_builder_connect_signals(builder, NULL);
        gtk_widget_show(window_chat_mini);
        Array_Chat_Name_Label[array_chat_name_count] = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Chat_Name"));

        gtk_label_set_text(GTK_LABEL(Array_Chat_Name_Label[array_chat_name_count]),friendname);
        array_chat_name_count++;
        }
   
  }

 void on_Btn_close_chat_clicked(GtkButton *button,GtkLabel *friend_chat_name)
  {
    GtkWidget *window_chat_mini_pointer = gtk_widget_get_toplevel (friend_chat_name);
    for(int i=0;i<array_chat_name_count;i++)
    {
       
        if(strcmp(gtk_label_get_text(friend_chat_name),gtk_label_get_text(Array_Chat_Name_Label[i]))==0)
        { 
            Array_Chat_Name_Label[i] = Array_Chat_Name_Label[array_chat_name_count-1];
            Array_Chat_Name_Label[array_chat_name_count-1]=NULL;
            array_chat_name_count--;
        }
    }
    gtk_widget_destroy(window_chat_mini_pointer);
  }


  

  void on_Btn_Send_clicked(GtkButton *button, GtkLabel *friend_chat_name)
  {
    GtkTextIter iter;
    GtkTextBuffer *buffer;

    char head[60]="MESS|";
    char sendline[MAXLINE];
    strcat(head,gtk_label_get_text(friend_chat_name));
    strcat(head,"|");
    GtkWidget *window_chat_mini_pointer = gtk_widget_get_toplevel (friend_chat_name);
    
    //Chu y cai nay phai la "GtkEntry"
    GtkWidget* entry_chat = find_child(window_chat_mini_pointer,"GtkEntry");
    strcat(head,gtk_entry_get_text(entry_chat));
    strcat(head,"\0");
    strcpy(sendline,head);
    send(server, sendline, strlen(sendline), 0);

    GtkWidget* view_chat = find_child(window_chat_mini_pointer,"GtkTextView");
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view_chat));
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, -1);
    gtk_text_buffer_insert(buffer,&iter,"you: ",-1);
    gtk_text_buffer_insert(buffer,&iter,gtk_entry_get_text(entry_chat),-1);
    gtk_text_buffer_insert(buffer,&iter,"\n",-1);
    

    gtk_entry_set_text(entry_chat,"");
}
  
  void reset_name_pass()
  {
      gtk_entry_set_text(entry_name,"");
      gtk_entry_set_text(entry_pass,"");
  }

  
  