#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

using namespace std;

extern int errno;

// This system demonstrates the use of the socket() message passing
// IPC mechanism.  Sockets are different from pipes and shared memory 
// because they can operate BETWEEN computers using the network.
//
// The purpose of the system is to provide a simple "chat" application.  
// The system consists of two programs "server" which sets up a TCP 
// port and accepts connections from clients.  And "client" which 
// connects to a TCP port to establish a connection with the server. 
// Both the client and server send messages back and forth in a 
// simplistic manner:
//
// SERVER:
// Loop until client types "/q":
//   Server: Wait for user to type in message
//   Server: Send message to client
//   Server: Wait for client to send message
//   Server: Display message from client
//
// CLIENT:
// Loop until "/q":
//   Client: Wait for server to send message
//   Client: Display message from server
//   Client: Wait for user to type in message
//   Client: Send message to client

int main(void)
{
  ////////////////////////////////////////////////////////////////////
  // Create a socket.  
  ////////////////////////////////////////////////////////////////////  
  int s = socket(PF_INET,SOCK_STREAM,0);

  if (s <= 0) {
    cerr << "clientr: Socket creation failed." << endl;
    exit(-1);
  }

  ////////////////////////////////////////////////////////////////////
  // Connect to server port
  ////////////////////////////////////////////////////////////////////
  sockaddr_in address;
  int addressSize = sizeof(sockaddr_in);
  address.sin_family=AF_INET;                         
  address.sin_port  = htons(15000); // <-------------------  port 15000
  inet_pton(AF_INET,"54.200.73.248",&address.sin_addr);  // <- IP 127.0.0.1 

  int status=0;
  status = connect(s,(struct sockaddr*) &address, addressSize);

  if (status != 0) {
    cerr << "client:Connect failed, can't connect to port." << endl;
    cout << strerror(errno) << endl;
    exit(-1);
  }

  ////////////////////////////////////////////////////////////////////
  // Send/Receive messages until client types "/q"
  ////////////////////////////////////////////////////////////////////
  cout << "Server and client have established communication." << endl;

  //
  // This local buffer will be used to copy messages to/from socket.
  // Sockets require a contiguous block of memory to copy messages.
  // This could be an array of characters, integers, objects, whatever.
  //
  // The format of messages between a client and server is called a
  // PROTOCOL.  As a programmer, YOU have to decide upon a protocol.
  //
  // To keep things simple with this example, we've created an array
  // of characters.  Messages are transmitted as C-Strings.  A message
  // always starts at buffer[0] and always ends at the first position 
  // in the buffer where a '\0' is encountered.
  //
  const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];

  do {

    // Wait for message from server
    cout << "client: Waiting for server message." << endl << endl;;
    recv(s,buffer,BUFFER_SIZE,0); 
    cout << "client: Server sent -> " << buffer << " <-" << endl << endl;

    // Quit if server message is "\q"
    if (strcmp(buffer,"/q")==0) break;

    // Get keyboard input from user
    cout << "client: Type in your message and press <ENTER>: ";
    cout.flush();
    cin.getline(buffer,BUFFER_SIZE);

    // Send message to server
    send(s,buffer,BUFFER_SIZE,0);

    // Stop when user types the string "\q"
  } while (strcmp(buffer,"/q"));

  // Cleanup
  close(s);

  return 1;
}
