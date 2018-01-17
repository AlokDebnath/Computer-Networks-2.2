#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
/* This is the server side of the code.
 * The connection request is sent from this side.
 * The files are sent from this side after the connection is established.
*/

typedef struct sockaddr_in socket_address; //sockadde_in is defined in netinet/in.h
socket_address server, client;
char buffer[256];

int main (int argc, char *argv[])
{
	int socketfd, acceptfd, portno, clientAddressLength; 
	int n;
    //socket_address server, client; //server = server address, client = client address
    
    if (argc < 2)
    {
        fprintf(stderr, "No port provided. Exiting");
        exit(1);
    }

    socketfd = socket(AF_INET, SOCK_STREAM, 0); //socket() is a system call. which creates a new socket. socketfd stores teh success or faliure
    if (socketfd < 0)
    {
        perror ("Socket could not be opened");
        exit(1);
    }
    
    portno = atoi(argv[1]);
    server.sin_family = AF_INET; //server address has 4 fields. The sin_family field decides whether it is the serer or not
    server.sin_port = htons(portno); //port number is a short of the network byte order, so must be converted hence the htons() function
    server.sin_addr.s_addr = INADDR_ANY;
    if (bind(socketfd, (struct sockaddr *) &server, sizeof(server)) < 0) //Bind an address to a value
    {
        perror ("Binding error");
        exit(1);
    }
    listen (socketfd, 5); //This makes sure that the port is not in use

    clientAddressLength = sizeof(client);
    acceptfd = accept(socketfd, (struct sockaddr *) &client, &clientAddressLength);
    if (acceptfd < 0)
    {
        perror ("Accept error occured");
        exit(1);
    }

    n = read (acceptfd, buffer, 255);
    if (n < 0)
    {
        perror("Error in reading from socket");
        exit(1);
    }

    printf("The message is: \n%s\n", buffer);
    
    n = write(acceptfd, "Message Received", 16);
    if (n < 0)
    {
        perror("Writing error");
        exit(1);
    }

    return 0;
}
