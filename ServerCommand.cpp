#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <DB.h>
#include <GETPUT.h> 

using namespace std;
const int S_ACK_SIZE = 1000;
char acSAckBuffer [S_ACK_SIZE];

string currentUser = "<USERNAME>";
extern bool isAuthenticated;
/*
	recv command occurs prior to any of this
	
	send ACK 
	OR
	send responseSize
	recv ACK
	send response
	...
	recv param size
	send ACK
	recv param
	send responseSize
	recv ACK
	send response
*/

string safeRecvPARAM(int cs) {
	cout << "ServerCommand:safeRecvPARAM" << endl;
	int lengthOfParam;

	recv(cs,&lengthOfParam,sizeof(lengthOfParam),0);
	
	const char* acknowledgement1 = "PARAM_LENGTH_ACK";
	send(cs,acknowledgement1,sizeof(acknowledgement1),0);

	cout << "ServerCommand:safeRecvPARAM: lengthOfParam: " << lengthOfParam << endl;
	char param [lengthOfParam];
	recv(cs,param,lengthOfParam,MSG_WAITALL);

	string returnParam = param;

	return returnParam.substr(0,lengthOfParam);
}
void safeResponse(string tempResponse, int cs) {
	const char* response = tempResponse.c_str();
	int responseSize = strlen(response);

	send(cs,&responseSize,sizeof(responseSize),0);

	bzero(&acSAckBuffer,S_ACK_SIZE);
	recv(cs,acSAckBuffer,S_ACK_SIZE,0);

	send(cs,response,responseSize,0);	
}
void safeCommandACK(int cs) {
	const char* acknowledgement = "CMD_ACK";
	send(cs,acknowledgement,sizeof(acknowledgement),0);
}
void serverHelp(int cs) {	
	int responseSize = HELP_LIST_LENGTH + 1;
	send(cs,&responseSize,sizeof(responseSize),0);
	
	bzero(&acSAckBuffer,S_ACK_SIZE);
	recv(cs,acSAckBuffer,S_ACK_SIZE,0);
	
	const char* response = HELP_LIST.c_str();
	send(cs,response,responseSize,0);
}
void serverUser(int cs){	
	safeCommandACK(cs);

	currentUser = safeRecvPARAM(cs);
		
	string tempResponse = "serverUser: Enter password for " + currentUser;
	safeResponse(tempResponse,cs);
}
void serverPassword(int cs){
	safeCommandACK(cs);

	string password = safeRecvPARAM(cs);
	isAuthenticated = authenticate(currentUser,password);

	string tempResponse;
	if(isAuthenticated) tempResponse = currentUser + " successfully authenticated";
	else tempResponse = "incorrect username or password";
	safeResponse(tempResponse,cs);

	bzero(&acSAckBuffer,S_ACK_SIZE);
	recv(cs,acSAckBuffer,S_ACK_SIZE,0);

	send(cs,&isAuthenticated,sizeof(isAuthenticated),0);
}
void serverDIR(MyDIR *directory, int cs){
	string tempResponse = (*directory).getCurrentPath() + 
	"\n\n" +(*directory).getCurrentDIR();
	
	int responseSize = tempResponse.length() + 1;
	send(cs,&responseSize,sizeof(responseSize),0);
	
	bzero(&acSAckBuffer,S_ACK_SIZE);
	recv(cs,acSAckBuffer,S_ACK_SIZE,0);
	
	const char* response = tempResponse.c_str();
	send(cs,response,responseSize,0);
}
void serverCD(MyDIR *directory, int cs) {
	safeCommandACK(cs);

	string nextDIR = safeRecvPARAM(cs);
	bool hasChanged = (*directory).changeToDIR(nextDIR);

	string tempResponse;
	if(hasChanged) tempResponse = "SUCCESS: changed server directory to: \n" + (*directory).getCurrentPath();
 	else tempResponse = "FAILURE: server directory not changed: \n " + (*directory).getCurrentPath();
	safeResponse(tempResponse,cs);
 }
void serverDEL(MyDIR *directory, int cs){
	safeCommandACK(cs);

	string fileToDelete = (*directory).getCurrentPath() + "/" + safeRecvPARAM(cs);

	string tempResponse;
	if(remove(fileToDelete.c_str()) == 0) {
    	tempResponse = fileToDelete+" successfully deleted.";
  	} else {
    	tempResponse= " something went wrong. Nothing deleted.";    
  	}
	safeResponse(tempResponse,cs);
}
void serverGet(MyDIR *directory, int cs){
	safeCommandACK(cs);

	string filename = safeRecvPARAM(cs);
	bool putSuccess = put(directory,filename,cs);

	string tempResponse;
	if(putSuccess)
		tempResponse = filename + " successfully gotten.";
	else {
		tempResponse = filename + " unsuccessfully gotten.";
	} 
	safeResponse(tempResponse,cs);
}
void serverPut(MyDIR *directory, int cs){
	safeCommandACK(cs);

	string filename = safeRecvPARAM(cs);
	bool getSuccess =get(directory,filename,cs);

	string tempResponse;
	if(getSuccess)
		tempResponse = filename + " successfully put.";
	else {
		tempResponse = filename + " unsuccessfully put.";
	} 
	safeResponse(tempResponse,cs);	
}
bool serverExit(int cs, int s) {
	cout << "ServerCommand:serverExit: exiting..." << endl;
	close(cs);
	close(s);
	return false;
}
