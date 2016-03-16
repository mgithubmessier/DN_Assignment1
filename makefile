CC = g++
CPPFLAGS = -g -I.
all: ftpClient ftpServer

ftpClient: ftpClient.o Command.o Message.o ClientCommand.o ServerCommand.o DIR.o DB.o GETPUT.o
ftpServer: ftpServer.o Command.o Message.o ClientCommand.o ServerCommand.o DIR.o DB.o GETPUT.o

Command.o: Command.cpp 
Message.o: Message.cpp 
ftpServer.o: ftpServer.cpp
ftpClient.o: ftpClient.cpp
ClientCommand.o: ClientCommand.cpp 
ServerCommand.o: ServerCommand.cpp 
DIR.o: DIR.cpp
DB.o: DB.cpp	
GETPUT.o: GETPUT.cpp

clean:
	rm BIG_FILE.txt
	rm SMALL_FILE.txt
	rm ftpServer
	rm ftpClient
	rm *.o
