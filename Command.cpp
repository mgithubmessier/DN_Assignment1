#include <iostream>

#include <Command.h>

using namespace std;

extern bool isAuthenticated;

string capitalize(string command) {
	for(int i = 0; i < command.length(); i++) {
		command[i] = toupper(command[i]);
	}
	return command;
}
//client parses its command here and sends the opcode to the server
int parseCommand(string command) {
	//cout << "Command:parseCommand" << endl;
	command = capitalize(command);
	//cout << "parseCommand:parsed.capitalized command: " << command << endl;
	if(command == "HELP") 			return HELP;
	else if(command == "USER") 		return USER;
	else if(command == "PASSWORD") 	return PASSWORD;
	else if(command == "DIR") 		return DIR;
	else if(command == "CD") 		return CD;
	else if(command == "LCD") 		return LCD;
	else if(command == "DEL") 		return DEL;
	else if(command == "GET") 		return GET;
	else if(command == "PUT") 		return PUT;
	else if(command == "EXIT") 		return EXIT;
	else return INV_CMD;
}
//server runs the recv'd command
//since recv'ing a parameter is conditional, perform recv in functions
bool serverRunCommand(int command, MyDIR *directory, int cs, int s) {
	//cout << "Command:serverRunCommand" << endl;
	switch (command) {
		case HELP: serverHelp			(cs);	break;
		case USER: serverUser			(cs);	break;
		case PASSWORD: serverPassword	(cs);	break;
		case DIR: serverDIR				(directory, cs);	break;
		case CD: serverCD 				(directory, cs);	break;
		case DEL: serverDEL				(directory, cs);	break;
		case GET: serverGet 			(directory, cs);	break;
		case PUT: serverPut 			(directory, cs);	break;
		case EXIT: return serverExit	(cs,s);
		default: cout <<"Command:runCommand:SERVER: INV_CMD" << endl;
	}
	return true;
}
bool clientRunCommand(Message msg, MyDIR *directory, int cs) {
	//cout << "Command:clientRunCommand" << endl;
	//cout << "Command:isAuthenticated: " << isAuthenticated << endl;
	if((msg.cmd == HELP) || (msg.cmd == USER) || (msg.cmd == PASSWORD) || (msg.cmd == EXIT) || (msg.cmd == LCD) ||
		(isAuthenticated)) {
		switch (msg.cmd) {
			case HELP: clientHelp			(msg,cs);			break;
			case USER: clientUser			(msg,cs);			break;
			case PASSWORD: clientPassword	(msg,cs);			break;
			case DIR: clientDIR				(directory,msg,cs);	break;
			case CD: clientCD 				(directory,msg,cs);	break;
			case LCD: clientLCD 			(directory,msg,cs);	break;
			case DEL: clientDEL				(directory,msg,cs);	break;
			case GET: clientGet				(directory,msg,cs);	break;
			case PUT: clientPut				(directory,msg,cs);	break;
			case EXIT: return clientExit	(msg, cs);
			default: cout <<"Command.cpp:runCommand:CLIENT: INV_CMD" << endl;
		}
	} else {
		cout << "User is not authenticated.\nOnly HELP, USER, PASSWORD, LCD, and EXIT command are available." << endl;
	}
	return true;
}



