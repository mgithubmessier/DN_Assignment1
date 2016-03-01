#include <string>
#include <DIR.h>
#include <Message.h>
void serverHelp		(int cs);
void serverUser		(int cs);
void serverPassword	(int cs);
void serverDIR		(MyDIR *directory, int cs);
void serverCD 		(MyDIR *directory, int cs);
void serverDEL 		(MyDIR *directory, int cs);
void serverGet 		(MyDIR *directory, int cs);
void serverPut 		(MyDIR *directory, int cs);
bool serverExit 	(int cs, int s);


void clientHelp 	(Message msg, int cs);
void clientUser		(Message msg, int cs);
void clientPassword (Message msg, int cs);
void clientDIR		(MyDIR *directory, Message msg, int cs);
void clientCD 		(MyDIR *directory, Message msg, int cs);
void clientLCD 		(MyDIR *directory, Message msg, int cs);
void clientDEL		(MyDIR *directory, Message msg, int cs);
void clientGet 		(MyDIR *directory, Message msg, int cs);
void clientPut 		(MyDIR *directory, Message msg, int cs);
bool clientExit 	(Message msg, int cs);

bool clientRunCommand(Message msg, MyDIR *directory, int cs);
bool serverRunCommand(int command, MyDIR *directory, int cs, int s);
int parseCommand(string command);


const int 	SERVER = 0,
			CLIENT = 1,
			INV_CMD = -1,
			HELP = 0,
			USER = 1,
			PASSWORD = 2,
			DIR = 3,
			CD = 4,
			LCD = 5,
			DEL = 6,
			GET = 7,
			PUT = 8, 
			EXIT = 9;
const string NO_PARAM = "!!NO..PARAM@@",
HELP_LIST = "HELP: lists ftp commands with explanations\nUSER: specifies a user for authentication\nPASSWORD: specifies a users password for authentication\nDIR: lists the files in the servers current *directory\nCD <path>: changes the servers current *directory to the new path.  The path can be relative or absolute\nLCD <path: changes the clients current *directory to the new path.  The path can be relative or absolute\nDEL <filename>: deletes the server file specified by <filename> in the current *directory\nGET <filename>: gets the server file specified by <filename> in the current *directory and places the file in the client *directory using the same <filename>\nPUT <filename>: puts the client file specified by <filename> in the current *directory and places the file in the server *directory using the same <filename>\nEXIT: end the session";
const int HELP_LIST_LENGTH = HELP_LIST.length();
