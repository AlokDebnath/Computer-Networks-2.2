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
char filename[1024] = {0};
char response[100] = {0};
char continueSend[10] = {0};

int main(int argc, char const *argv[])
{
  struct sockaddr_in address;
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char *hello = "Hello from client";
  char *done = "Done";
  int i, stillRead=1;

  while(1){  
  
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
  printf("Connection Opened\n");
  // Below is the Interaction that we need to worry about. Above is all boilerplate.

  /*File Transfer Below*/


    //Response with file name
    printf("Enter file name: ");
    fflush(stdout);
    scanf("%s", filename);
    send(sock, filename, strlen(filename), 0);
  
  
    //This is where we hopefully receive Sending... from the server
    for(i = 0; i < 1024; response[i++] = '\0');
    valread = read(sock, buffer, 1024);
    for(i = 0; i < valread; response[i] = buffer[i], i++);
    response[valread] = '\0';

    //If Sending, then prepare to write into the file
    if (strcmp(response, "Sending...") == 0)
      {
        FILE* fp = fopen(filename, "wb"); //Create the file
        send(sock, "Ok", strlen("Ok"), 0); //Response cycle, to help with writing.
        while(stillRead)
          {
            valread = read(sock, buffer, 1024);
            // Exiting the Writing Loop when Server says Done.
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
    else {
      printf("%s\n", response);
    }
    printf("Ask for another file?(Yes/No): ");
    fflush(stdout);
    scanf("%s", continueSend);
    if (strcmp(continueSend, "Yes") == 0 || strcmp(continueSend, "yes") == 0 || strcmp(continueSend, "Y") == 0 || strcmp(continueSend, "y") == 0) {
      send(sock, "Sending More", strlen("Sending More"), 0);
      valread = read(sock, buffer, 1024);
      for(i = 0; i < valread; printf("%c", buffer[i++]));
      printf("\n");
      stillRead = 1;
    }
    else{
      send(sock, "No More", strlen("No More"), 0);
      valread = read(sock, buffer, 1024);
      break;
    }
    printf("Closing Connection\n");
    fflush(stdout);
  }
    
  return 0;
}
