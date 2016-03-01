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

Message parseMessage(string message) {
	Message msg;
	int iFirstNonSpace = message.find_first_not_of(" ");
	message = message.substr(iFirstNonSpace);
	int iFirstSpace = message.find(" ");
	
	//cout << "parseMessage:prefixed whitespace removed: " << message << endl;

	if(iFirstSpace != string::npos) {
		string command = message.substr(0,iFirstSpace);

		//cout << "parseMessage:command(string): " << command << endl;

		msg.cmd = parseCommand(command);

		//cout << "parseMessage:command(int): " << msg.cmd << endl;
		if(msg.cmd != INV_CMD) {
			msg.parameter = message.substr(iFirstSpace+1);
		} else {
			cout << "Message.cpp:parseMessage: invalid command" << endl;
		}
	} else {
		msg.cmd = parseCommand(message);
		msg.parameter = NO_PARAM;
	}
	return msg;
}

void serverCommLoop(int cs, int s) {
	//receive integer representation of command
	int command;
	MyDIR *directory = new MyDIR();
	do {
		recv(cs,&command,sizeof(command),0);
	} while(serverRunCommand(command, directory,cs, s));
}

void clientCommLoop(int s) {
	//Begin loop
	string message;
	MyDIR *directory = new MyDIR();
	do {
		cout << "Enter message: ";
		getline(cin,message); 
	} while(clientRunCommand(parseMessage(message),directory,s));
}

