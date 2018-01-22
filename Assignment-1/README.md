# Computer Networks Assignment-1

## Details of Developers
   Name: Projit Bandyopadhyay
   Roll Number: 20161014
   Name: Alok Debnath
   Roll Number: 2011122
   
## Theory

+ What is HTTP?
  HTTP or Hypertext Transfer Protocol is an application transfer protocol and is the basis of communication over the web. It follows a request-response protocol for a model that has a server and a client. An HTTP request is submitted to the	server, which then provides the resources to the client.

+ State of a HTTP transfer
  HTTP is stateless, which means that the server does not store the information or the status about each user or the duration of requests.

+ Persistent versus non-persistent connection
  A persistent connection is one where the connection is "kept-alive" after the transfer has been completed. This means that the connection does not autmoatically get shut down as soon as the file has been transferred. The idea is to use a single TCP connection to send/recieve multiple HTTP requests/responses
  A non-persistent connection is one that closes the connection after each file has been transferred and to transfer a new file a new TCP connection and set it up in order to send/receive multiple HTTP requests/responses

## Running the Persistent Part
   We have coded a server and a client for a persistent and a non-persistent connection. The file structure is as follows:
   Assignment-1
	|- Persistent
	|	|- Server
	|	|- Client
	|- Non-persistent
	|	|- ServerNonPersis
	|	|- ClientNonPersis

   The instructions to run are as follows:
   1. Open terminal (Ctrl-Alt-T)
   2. Run the following commands:
   ls (to find out the files needed)
   cd Assignment-1/Persistent/Server
   gcc fileserver.c
   ./a.out
   3. Open another terminal tab/window and DO NOT CLOSE the server tab/window
   4. Run the following commands:
   cd Assignment-1/Persistent/Client
   gcc fileclient.c
   ./a.out
   5. When prompted input the file name to transfer (file1.txt, file2.txt, adventur.txt, SampleJPGImage_2mbmb.jpg).
   6. If you want to transfer any more files, when prompted, type Yes or Y. Repeat 5 if yes. Else type No or N.
   7. On the server and client side, run:
   cat <filename>

The acid test we used was to transfer a smaple 2mb picture to see if the transfer was taking place correctly and the pictures after transfer are identical.

## Running the Non-persistent Part
   All instructions are the same and the files are also the same except when instead of "cd Persistent/Server/" do "cd Non-Persistent/ServerNonPersis". Similarly for client.
