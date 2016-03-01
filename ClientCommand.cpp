#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#include <GETPUT.h>
const int C_ACK_SIZE = 1000;
char acCAckBuffer [C_ACK_SIZE];

bool isAuthenticated = false;

/*
	send command
	recv ACK
	OR
	recv responseSize
	send ACK
	recv responseSize
	...
	send param size
	recv ACK
	send param

	recv responseSize
	send ACK
	recv response
*/

void safeSendPARAM(Message msg, int cs) {
	int iPARAMSize = msg.parameter.length();
	send(cs,&iPARAMSize,sizeof(iPARAMSize),0);
	
	bzero(&acCAckBuffer,C_ACK_SIZE);
	recv(cs,acCAckBuffer,C_ACK_SIZE,0);

	send(cs,msg.parameter.c_str(),iPARAMSize,MSG_WAITALL);
}
void safeExchange(Message msg, int cs) {

	send(cs,&msg.cmd,sizeof(msg.cmd),0);

	recv(cs,acCAckBuffer,C_ACK_SIZE,0);

	safeSendPARAM(msg,cs);

	int responseSize;
	recv(cs,&responseSize,sizeof(responseSize),0);

	const char* acknowlegement = "RES_SIZE_ACK";
	send(cs,acknowlegement,sizeof(acknowlegement),0);

	char response [responseSize];
	recv(cs,response,responseSize,0);

	string exactResponse = response;
	exactResponse = exactResponse.substr(0,responseSize);
	cout << "\n\n" << exactResponse << "\n\n" << endl;
}
void clientHelp(Message msg, int cs) {
	send(cs,&msg.cmd,sizeof(msg.cmd),0);

	int responseSize;
	recv(cs,&responseSize,sizeof(responseSize),0);
	
	const char* acknowlegement = "RES_SIZE_ACK";
	send(cs,acknowlegement,sizeof(acknowlegement),0);

	char response [responseSize];
	recv(cs,response,responseSize,0);

	cout << "\n\n" << response << "\n\n" << endl;
}
void clientUser(Message msg, int cs) {
	safeExchange(msg,cs);
}
void clientPassword(Message msg, int cs){
	safeExchange(msg,cs);
	
	const char* acknowlegement = "AUTH_ACK";
	send(cs,acknowlegement,sizeof(acknowlegement),0);

	recv(cs,&isAuthenticated,sizeof(isAuthenticated),0);
}
void clientDIR(MyDIR *directory , Message msg, int cs){
	send(cs,&msg.cmd,sizeof(msg.cmd),0);

	int responseSize;
	recv(cs,&responseSize,sizeof(responseSize),0);
	
	const char* acknowlegement = "RES_SIZE_ACK";
	send(cs,acknowlegement,sizeof(acknowlegement),0);

	char response [responseSize];
	recv(cs,response,responseSize,0);

	cout << "\n\n" << response << "\n\n" << endl;
}
void clientCD(MyDIR *directory , Message msg, int cs){
	safeExchange(msg,cs);
}
void clientLCD(MyDIR *directory , Message msg, int cs){
	if((*directory).changeToDIR(msg.parameter))
		cout << "SUCCESS: changed client directory to: \n" + (*directory).getCurrentPath() << endl;
	else 
		cout << "FAILURE: client directory not changed: \n" + (*directory).getCurrentPath() << endl;
}
void clientDEL(MyDIR *directory , Message msg, int cs){
	safeExchange(msg,cs);
}
void clientGet(MyDIR *directory , Message msg, int cs){
	send(cs,&msg.cmd,sizeof(msg.cmd),0);

	recv(cs,acCAckBuffer,C_ACK_SIZE,0);

	string filename = msg.parameter;
	safeSendPARAM(msg,cs);
	bool gettSuccess = get(directory,filename,cs);

	int responseSize;
	recv(cs,&responseSize,sizeof(responseSize),0);

	const char* acknowlegement = "RES_SIZE_ACK";
	send(cs,acknowlegement,sizeof(acknowlegement),0);

	char response [responseSize];
	recv(cs,response,responseSize,0);

	string exactResponse = response;
	exactResponse = exactResponse.substr(0,responseSize);
	cout << "\n\n" << exactResponse << "\n\n" << endl;
}
void clientPut(MyDIR *directory , Message msg, int cs){
	send(cs,&msg.cmd,sizeof(msg.cmd),0);

	recv(cs,acCAckBuffer,C_ACK_SIZE,0);
	
	string filename = msg.parameter;
	safeSendPARAM(msg,cs);
	bool gettSuccess = put(directory,filename,cs);
	
	int responseSize;
	recv(cs,&responseSize,sizeof(responseSize),0);

	const char* acknowlegement = "RES_SIZE_ACK";
	send(cs,acknowlegement,sizeof(acknowlegement),0);

	char response [responseSize];
	recv(cs,response,responseSize,0);

	string exactResponse = response;
	exactResponse = exactResponse.substr(0,responseSize);
	cout << "\n\n" << exactResponse << "\n\n" << endl;
}
bool clientExit(Message msg, int cs) {
	cout << "ClientCommand:clientExit: exiting..." << endl;
	send(cs,&msg.cmd,sizeof(msg.cmd),0);
	close(cs);
	return false;
}