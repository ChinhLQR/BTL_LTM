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

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 3000
#define MESSAGE_LENGTH 300
#define MAXLINE 4096


GtkBuilder      *builder; 

//Cua so login
GtkWidget       *window;
GtkWidget       *entry_name;
GtkWidget       *entry_pass;
GtkWidget       *label;

//Cua so chat
GtkWidget       *view_chat;
GtkWidget       *entry_chat;
GtkWidget	      *entry_add;
GtkWidget       *entry_del;
GtkWidget       *window_friend;
GtkWidget       *window_notice;
GtkWidget       *label_friend;
GtkWidget       *label_notice;
GtkWidget       *label_myname;
GtkWidget       *entry_friend;
GtkWidget       *label_chat_name;

GtkTextIter iter;
GtkTextBuffer *buffer;

char username[20];
char friendname[20];

char friend_chat_current[20];
char friend_chat[20];
int server = -1;


//send
int send_message(int server, char* message) {
  ssize_t sent_length;
  sent_length = send(server, message, strlen(message), 0);
  return (int)sent_length;
}

//recv
int recv_message(int server, char* message) {
  ssize_t sent_length;
  sent_length = recv(server, message, strlen(message), 0);
  printf("INFO: Sent %d byte(s)\n", (int)(sent_length));
  return (int)sent_length;
}


//connect to server
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




void signio_handler(int signo)
  {
    GtkBuilder      *builder; 
    
    builder = gtk_builder_new();
    char user[20],pass[20];
    
    char* p;
    char  *recvline;
    


    
    char buff[1024];
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
        gtk_builder_add_from_file (builder, "window_main.glade", NULL); 
        gtk_widget_hide(window);
        window = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Chat"));
        gtk_builder_connect_signals(builder, NULL);
        view_chat = GTK_WIDGET(gtk_builder_get_object(builder,"View_Chat"));
        entry_chat = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_Chat"));
        entry_friend = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_Friend"));
        entry_add = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_AddFriend"));
        entry_del = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_DeleteFriend"));
        label_myname = GTK_WIDGET(gtk_builder_get_object(builder,"Label_MyName"));
        char welcome[60] = "WelCome ";
        strcat(welcome,username);
        gtk_label_set_text(GTK_LABEL(label_myname),welcome);

        
        gtk_widget_show(window);
        printf("login success\n");
    
      }

      if (strcmp(p,"202")==0){}

      if (strcmp(p,"203")==0){}

      if (strcmp(p,"204")==0){}


      if (strcmp(p,"205")==0)
      {
        gtk_label_set_text(GTK_LABEL(label),"Login Failed , try again !");
        reset_name_pass();
        printf("login error\n");
        
      }
      /////////////////// End Tin hieu tu login






      ////////////////////Tin hieu tu Dang Xuat
      if (strcmp(p,"301")==0){}
      if (strcmp(p,"302")==0){}
      if (strcmp(p,"303")==0){}
      //////////////////// End Tin hieu Dang Xuat







      ////////////////TIn hieu tu yeu cau Ket Ban
      ///ko in ra label ten ban dc

      //401:da gui/403:gui that bai
      if (strcmp(p,"401")==0||strcmp(p,"403")==0){
        gtk_builder_add_from_file (builder, "window_main.glade", NULL); 
        window_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Notice"));
        gtk_builder_connect_signals(builder, NULL);
        gtk_widget_show(window_notice);
        label_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Notice"));
        p = strtok(NULL, "|");
        gtk_label_set_text(GTK_LABEL(label_notice),p);
        
      }
      //402: pop_up yeu cau ket ban
      if (strcmp(p,"402")==0){
        gtk_builder_add_from_file (builder, "window_main.glade", NULL); 
        window_friend = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Friend"));
        label_friend = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Friend"));
        gtk_builder_connect_signals(builder, NULL);
         p = strtok(NULL, "|");
         strcpy(friendname,p);
        // label_friend = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Friend"));
        // gtk_builder_add_from_file (builder, "window_main.glade", NULL); 
        // window_friend = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Friend"));
        // gtk_builder_connect_signals(builder, NULL);
    
        gtk_widget_show(window_friend);
         char dialog[60] = "Friend Request from ";
         strcat(dialog,p);
        
         gtk_label_set_text(GTK_LABEL(label_friend),dialog);
        
        }

     
      //////////////////End tin hieu Yeu cau Ket Ban
      
      

      ///////////////////Tin hieu tu chap nhan ket ban
      //501: gui thanh cong/ 502:
      if (strcmp(p,"501")==0||strcmp(p,"502")==0 || strcmp(p,"503")==0 ){
        gtk_builder_add_from_file (builder, "window_main.glade", NULL); 
        window_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Notice"));
        gtk_builder_connect_signals(builder, NULL);
        g_object_unref(builder);
        label_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Notice"));
        gtk_widget_show(window_notice);
        p = strtok(NULL, "|");
        gtk_label_set_text(GTK_LABEL(label_notice),p);
      }
      /////////////////////End tin hieu chap nhan ket ban

      

      ///////////////////Tin hieu tu Tu Choi Ket Ban
      if (strcmp(p,"601")==0||strcmp(p,"602")==0){
        gtk_builder_add_from_file (builder, "window_main.glade", NULL); 
        window_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Notice"));
        gtk_builder_connect_signals(builder, NULL);
        g_object_unref(builder);
        label_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Notice"));
        gtk_widget_show(window_notice);
        p = strtok(NULL, "|");
        gtk_label_set_text(GTK_LABEL(label_notice),p);
      }
      
      ///////////////////End tin hieu Tu Choi Ket Ban




      ///////////////////Tin hieu tu Xoa Ban
      if (strcmp(p,"701")==0||strcmp(p,"702")==0||strcmp(p,"703")==0){
        gtk_builder_add_from_file (builder, "window_main.glade", NULL); 
        window_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Notice"));
        gtk_builder_connect_signals(builder, NULL);
        g_object_unref(builder);
        label_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Notice"));
        gtk_widget_show(window_notice);
        p = strtok(NULL, "|");
        
        gtk_label_set_text(GTK_LABEL(label_notice),p);
      }

      ///////////////////End tin hieu tu Xoa Ban




      //////////////////Tin hieu tu Gui Tin Nhan
      if (strcmp(p,"801")==0){}
      if (strcmp(p,"802")==0){
        p = strtok(NULL, "|");
        char *friend = p;
        p = strtok(NULL,"|");
        char *mess = p;

        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view_chat));
        gtk_text_buffer_get_iter_at_offset(buffer, &iter, -1);
    
        gtk_text_buffer_insert(buffer,&iter,friend,-1);
        gtk_text_buffer_insert(buffer,&iter,": ",-1);
        gtk_text_buffer_insert(buffer,&iter,mess,-1);
        gtk_text_buffer_insert(buffer,&iter,"\n",-1);
        
    
        gtk_text_view_set_buffer(view_chat,buffer);
      }
      if (strcmp(p,"803")==0){
        gtk_builder_add_from_file (builder, "window_main.glade", NULL); 
        window_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Notice"));
        gtk_builder_connect_signals(builder, NULL);
        gtk_widget_show(window_notice);
        label_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Notice"));
        p = strtok(NULL, "|");
        gtk_label_set_text(GTK_LABEL(label_notice),p);
      }
      //////////////////End tin hieu tu Gui Tin Nhan
    }
   
  


  }

int main(int argc, char *argv[])
{   
   //connect server       
   int server = connect_server("127.0.0.1", 8484);
   
   //Non-block signal
   if(fcntl(server, F_SETFL, O_NONBLOCK|O_ASYNC))
   printf("Error in setting socket to async, nonblock mode");  

   signal(SIGIO, signio_handler); 

   if (fcntl(server,F_SETOWN, getpid()) <0)
   printf("Error in setting own to socket");
    
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
    entry_add = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_AddFriend"));
    entry_del = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_DeleteFriend"));
    label_friend = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Friend"));
    label_notice = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Notice"));
    label_myname = GTK_WIDGET(gtk_builder_get_object(builder, "Label_MyName"));
    entry_friend = GTK_WIDGET(gtk_builder_get_object(builder,"Entry_Friend"));
    label_chat_name = GTK_WIDGET(gtk_builder_get_object(builder, "Label_Chat_Name"));

   


    gtk_builder_connect_signals(builder, NULL);
	
    g_object_unref(builder);
    gtk_widget_show(window); 
    
  
    gtk_main();

    return 0;
}
 

// called when window is closed
void on_window_main_destroy()
{   
	int bytes_recv;
 	char sendline[MAXLINE], recvline[MAXLINE];
	char head[60]="LGOT|";		
    
  strcat(head,username);
	strcat(head,"\0");
	send(server, head, strlen(head), 0);
	bytes_recv = recv(server, head, MAXLINE,0);
  strcpy(username,"");
  gtk_main_quit();
	
}


//Xu li Login
void on_Btn_Login_clicked(GtkButton *button,GtkWindow *window_login)
{
    GtkBuilder      *builder; 
    //GtkWidget       *window;
    builder = gtk_builder_new();
    char user[20],pass[20];
    char head[60]="LGIN|";
    char* p;
    char sendline[MAXLINE], recvline[MAXLINE];
    int bytes_recv;

    pthread_t tid[2];

    strcat(username,gtk_entry_get_text(entry_name));
    strcat(head,gtk_entry_get_text(entry_name));
    strcat(head,"|");
    strcat(head,gtk_entry_get_text(entry_pass));
    strcat(head,"\0");
    strcpy(sendline,head);
    send(server, sendline, strlen(sendline), 0);
    bytes_recv = recv(server, recvline, MAXLINE,0);
  

}

//Xu li SignUp
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

void on_Btn_Chat_clicked()
{
  
  // GtkBuilder      *builder; 
  // builder = gtk_builder_new();
  //  gtk_builder_add_from_file (builder, "window_main.glade", NULL); 
  //   GtkWidget *window_chat_mini = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Chat_Mini"));
  //  gtk_builder_connect_signals(builder, NULL);
  //   g_object_unref(builder);
    
  //   gtk_widget_show(window_chat_mini);
    
  //   char label[20];
  //   strcpy(label,gtk_label_get_text (label_chat_name));
  //  printf("hello %s\n\n",label);
    
   
   
  //  gtk_label_set_text(GTK_LABEL(label_chat_name),"haha");
    
    
  //   char labelx[20];
  //      strcpy(labelx,gtk_label_get_text (label_chat_name));
  //     printf("hello %s\n\n",labelx);
    
    
    
     
  
  
}
void on_Btn_Send_clicked()
{
  char head[60]="MESS|";
  char sendline[MAXLINE];
    //de day thi se bi ko xuong dong ma len nguoc, nen vut len tren khi vua chuyen window
   // GtkTextIter iter;
    //GtkTextBuffer *buffer;
    gtk_text_view_set_buffer(view_chat,buffer);
    strcat(head,gtk_entry_get_text(entry_friend));
    strcat(head,"|");
    strcat(head,gtk_entry_get_text(entry_chat));
    strcat(head,"\0");
    strcpy(sendline,head);
    send(server, sendline, strlen(sendline), 0);
    

   
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view_chat));
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, -1);
    gtk_text_buffer_insert(buffer,&iter,"you: ",-1);
    gtk_text_buffer_insert(buffer,&iter,gtk_entry_get_text(entry_chat),-1);
    gtk_text_buffer_insert(buffer,&iter,"\n",-1);
    

    gtk_entry_set_text(entry_chat,"");

  

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

void on_Btn_Accept_clicked()
{
  char head[60]="ACCR|";
  char* p;
  char sendline[MAXLINE];

  strcat(head,friendname);
  strcat(head,"\0");
  strcpy(sendline,head);
  send(server, sendline, strlen(sendline), 0);
  printf("%s\n",sendline);

  strcpy(friendname,"");
  gtk_widget_destroy(window_friend);
}

void on_Btn_Refuse_clicked()
{
  char head[60]="DECR|";
  char* p;
  char sendline[MAXLINE];

  strcat(head,friendname);
  strcat(head,"\0");
  strcpy(sendline,head);
  send(server, sendline, strlen(sendline), 0);
  printf("%s\n",sendline);

  strcpy(friendname,"");
  gtk_widget_destroy(window_friend);
}

void on_Btn_Notice_clicked()
{
  gtk_widget_destroy(window_notice);
}


void reset_name_pass()
{
    gtk_entry_set_text(entry_name,"");
    gtk_entry_set_text(entry_pass,"");
}



