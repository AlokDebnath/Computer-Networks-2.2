#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080

void clearBuffer(char *buffer)
{
  int i;
  for(i = 0; i < 1024; buffer[i++] = '\0');
  return;
}

int main(int argc, char const *argv[])
{
  int server_fd, new_socket, valread;
  struct sockaddr_in address;  // sockaddr_in - references elements of the socket address. "in" for internet
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  char *hello = "Hello from server\n";

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)  // creates socket, SOCK_STREAM is for TCP. SOCK_DGRAM for UDP
    {
      perror("socket failed");
      exit(EXIT_FAILURE);
    }

  // This is to lose the pesky "Address already in use" error message
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                 &opt, sizeof(opt))) // SOL_SOCKET is the socket layer itself
    {
      perror("setsockopt");
      exit(EXIT_FAILURE);
    }
  address.sin_family = AF_INET;  // Address family. For IPv6, it's AF_INET6. 29 others exist like AF_UNIX etc. 
  address.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP address - listens from all interfaces.
  address.sin_port = htons( PORT );    // Server port to open. Htons converts to Big Endian - Left to Right. RTL is Little Endian

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address,
           sizeof(address))<0)
    {
      perror("bind failed");
      exit(EXIT_FAILURE);
    }

  while(1){
  
  // Port bind is done. You want to wait for incoming connections and handle them in some way.
  // The process is two step: first you listen(), then you accept()
  if (listen(server_fd, 3) < 0) // 3 is the maximum size of queue - connections you haven't accepted
    {
      perror("listen");
      exit(EXIT_FAILURE);
    }

  // returns a brand new socket file descriptor to use for this single accepted connection. Once done, use send and recv
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t*)&addrlen))<0)
    {
      perror("accept");
      exit(EXIT_FAILURE);
    }

  //Above can be thought of as boilerplate.

  /*File Transfer Below*/
  char fileError[] = "File Couldn't be Accessed\n";
  
  

    //Receive what file to look for

    valread = read( new_socket , buffer, 1024);  
    int i,rd;
    char sFile[1024], rbuf[1024], continueSend[1024];
    for(i = 0; i < 16; rbuf[i++] = '\0');
    for(i = 0; i < valread; sFile[i] = buffer[i], i++);
    sFile[valread] = '\0';
    // Open the file
    FILE* f = fopen(sFile,"rb");
    if (!f){
      send(new_socket, fileError, strlen(fileError), 0);
      printf("Error Opening File\n"); //Error Handling
    }
    else{
      printf("Yay file opened\n");
      fflush(stdout);
      send(new_socket, "Sending...", strlen("Sending..."), 0); //Tell Client about to send
      valread = read(new_socket , buffer, 1024); //Receive confirmation from client

      //Sending the File
      while(rd = fread(rbuf, 1, 16, f) != 0)
        {
          send(new_socket, rbuf, 16, 0);
          valread = read(new_socket, buffer, 1024);
          clearBuffer(rbuf);
        }
      send(new_socket, "Done", strlen("Done"), 0); //Done Sending the File.
      fclose(f);
    }
    valread = read(new_socket, continueSend, 1024);
    continueSend[valread] = '\0';
    if(strcmp(continueSend, "No More") == 0)
      {
        send(new_socket, "Ok", strlen("Ok"), 0);
        break;
      }
    else if (strcmp(continueSend, "Sending More") == 0){
      send(new_socket, "Server Says Ok Send", strlen("Server Says Ok Send"), 0);
    }
  }
  printf("Closing Connection\n");
  fflush(stdout);
  return 0;
}
