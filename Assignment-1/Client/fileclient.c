// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
char buffer[1024] = {0};

int main(int argc, char const *argv[])
{
  struct sockaddr_in address;
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char *hello = "Hello from client";
  char *done = "Done";
  char filename[1024];
  char response[100];
  int i, stillRead=1;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      printf("\n Socket creation error \n");
      return -1;
    }

  memset(&serv_addr, '0', sizeof(serv_addr)); // to make sure the struct is empty. Essentially sets sin_zero as 0
  // which is meant to be, and rest is defined below

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Converts an IP address in numbers-and-dots notation into either a 
  // struct in_addr or a struct in6_addr depending on whether you specify AF_INET or AF_INET6.
  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
      printf("\nInvalid address/ Address not supported \n");
      return -1;
    }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  // connect to the server address
    {
      printf("\nConnection Failed \n");
      return -1;
    }

  // Below is the Interaction that we need to worry about. Above is all boilerplate.

  //Hello Interaction...good proof of concept
  send(sock , hello , strlen(hello) , 0 );  // send the message.
  printf("Hello message sent\n");
  valread = read( sock , buffer, 1024);  // receive message back from server, into the buffer
  printf("%s\n",buffer );

  /*File Transfer Below*/
    
  //Was asked which file client wants
  buffer[0] = '\0';
  valread = read( sock , buffer, 1024);
  printf("%s\n",buffer );


  //Response with file name
  printf("Enter file name: ");
  scanf("%s", filename);
  send(sock, filename, strlen(filename), 0);
  
  
  //Response
  buffer[0] = '\0';
  valread = read(sock, buffer, 1024);
  for(i = 0; i < valread; response[i] = buffer[i], i++);
  response[valread] = '\0';

  for(i = 0; i < valread; printf("%c", buffer[i++]));

  if (strcmp(response, "Sending...\n") == 0)
    {
      FILE* fp = fopen(filename, "wb");
      while(stillRead)
        {
          buffer[0] = '\0';
          valread = read(sock, buffer, 1024);
          //printf("%d\n", stillRead++);
          if(valread == 4){
            buffer[4] = '\0';
            if(strcmp(buffer, "Done") == 0){
              stillRead = 0;
              printf("Done\n");
              break;
            }
          }
          fseek(fp, 0, SEEK_END);
          fwrite(buffer, valread, 1, fp);
          send(sock, done, strlen(done), 0);
        }
    }
  return 0;
}
