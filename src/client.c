/*
* @Author: GiapMinhCuong-20140539
* @Date:   2017-11-01 02:11:19
* @Last Modified by:   GiapMinhCuong-20140539
* @Last Modified time: 2017-11-02 01:11:30
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 3000
#define MESSAGE_LENGTH 300
#define MAXLINE 4096

void signup();
void login();

int send_message(int server, char* message) {
  ssize_t sent_length;
  sent_length = send(server, message, strlen(message), 0);
  return (int)sent_length;
}

int recv_message(int server, char* message) {
  ssize_t sent_length;
  sent_length = recv(server, message, strlen(message), 0);
  printf("INFO: Sent %d byte(s)\n", (int)(sent_length));
  return (int)sent_length;
}

int connect_server(char* server_ip, int port) {
  // 1 Tao socket
  int server = socket(AF_INET, SOCK_STREAM, 0);
  if (server < 0) {
    printf("ERROR: socket()!\n");
    return 1;
  }

  // 2 Connect
  // 2.1 Tao server_address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  // cach 2 su dung inet_addr. Method nay khong duoc khuyen khich
  //server_address.sin_addr.s_addr = inet_addr(server_ip);
  inet_aton(server_ip, &(server_address.sin_addr));
  server_address.sin_port = htons(port);

  // 2.2 Connect
  int connect_result = connect(server, (struct sockaddr*)&server_address, sizeof(server_address));
  if (connect_result < 0) {
    printf("ERROR: connect()\n");
  }
  return server;
}

int main(int argc, char* argv[]) {

  int server = connect_server("127.0.0.1", 3000);
  int a;

  printf("nhap yeu cau\n");
  printf("1.sigup\n2.login\n");
  scanf("%d",&a);
  switch(a){
    case 1:
    signup();
    break;

    case 2:
    login();
    break;

    case 3:
    break;
}
}

void signup(){
int server = connect_server("127.0.0.1", 8484);
  char user[20],pass[20];
  char head[60]="SGNU|";
  char* p;
  char sendline[MAXLINE], recvline[MAXLINE];
  int bytes_recv;
  printf("user name: \n");
  scanf("%s",user);
  printf("password\n");
  scanf("%s",pass);
  strcat(head,user);
  strcat(head,"|");
  strcat(head,pass);
  strcat(head,"\0");
  strcpy(sendline,head);
  send(server, sendline, strlen(sendline), 0);

  bytes_recv = recv(server, recvline, MAXLINE,0);
  recvline[bytes_recv]='\0';
  
  p = strtok(recvline, "|");
  
  if (strcmp(p,"101")==0)
  {
    printf("sigup success\n");
  }
  if (strcmp(p,"102")==0)
  {
    printf("sigup error\n");
  }
}
void login(){
  int server = connect_server("127.0.0.1", 8484);
  char user[20],pass[20];
  char head[60]="LGIN|";
  char* p;
  char sendline[MAXLINE], recvline[MAXLINE];
  int bytes_recv;
  printf("user name: \n");
  scanf("%s",user);
  printf("password\n");
  scanf("%s",pass);
  strcat(head,user);
  strcat(head,"|");
  strcat(head,pass);
  strcat(head,"\0");
  strcpy(sendline,head);
  send(server, sendline, strlen(sendline), 0);
  bytes_recv = recv(server, recvline, MAXLINE,0);
  recvline[bytes_recv]='\0';
  printf("%s\n",recvline);
  p = strtok(recvline, "|");
  if (strcmp(p,"201")==0)
  {
    printf("login success\n");
  }
  if (strcmp(p,"205")==0)
  {
    printf("login error\n");
  }
}

