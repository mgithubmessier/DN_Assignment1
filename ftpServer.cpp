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
	cerr << "server: Socket creation failed." << endl;
	exit(-1);
	}

	sockaddr_in address;
	int addressSize = sizeof(sockaddr);
	address.sin_family=AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port  = htons(15003); 

	int status=0;
	status = bind(s,(struct sockaddr*) &address, addressSize);

	if (status != 0) {
	cerr << "server: Bind failed, unable to create port." << endl;
	exit(-1);
	}

	cout << "server: Waiting for client to connect" << endl;
	listen(s,3);
	int cs = accept(s,(struct sockaddr*) &address,(socklen_t *) &addressSize);

	if (cs <= 0) {
	cerr << "server: Accept failed, client can't connect to port." << endl;
	cout << strerror(cs) << endl;
	exit(-1);
	}

	cout << "Server and client have established communication." << endl;


	serverCommLoop(cs,s);

	return 0;
}
