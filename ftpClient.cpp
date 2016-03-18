#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#include <Command.h>

using namespace std;

int main() {
	int s = socket(PF_INET,SOCK_STREAM,0);

	if (s <= 0) {
	cerr << "clientr: Socket creation failed." << endl;
	exit(-1);
	}
	sockaddr_in address;
	int addressSize = sizeof(sockaddr_in);
	address.sin_family=AF_INET;                         
	address.sin_port  = htons(15004); // <-------------------  port 15000
	//amazon aws server
	//inet_pton(AF_INET,"52.36.181.172",&address.sin_addr); 
	//localhost
	inet_pton(AF_INET,"127.0.0.1",&address.sin_addr); 


	int status=0;
	status = connect(s,(struct sockaddr*) &address, addressSize);

	if (status != 0) {
	cerr << "client:Connect failed, can't connect to port." << endl;
	cout << strerror(errno) << endl;
	exit(-1);
	}
	cout << "Server and client have established communication." << endl;

	clientCommLoop(s);
 	
	return 0;
}
