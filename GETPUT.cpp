#include <GETPUT.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

using namespace std;
bool put(MyDIR *directory, string filename, int cs) {
    string content,line;
    ifstream inputFile; 
    string filePath = ((*directory).getCurrentPath() + "/" + filename);
    inputFile.open(filePath, ios_base::binary);
    cout << "GETPUT:put: reading and sending "<< filename << endl;
    if (inputFile.is_open())
    {
        cout << "\n\n\n\n" << endl;

        //get length of file
        inputFile.seekg(0, inputFile.end);
        int lengthOfFile = inputFile.tellg();
        inputFile.seekg(0, inputFile.beg);

        int length = 1024;
        char ackArray [1000];
        char buffer [1024];
        bool continueGetting = true;

        for(int i = 1024; i < lengthOfFile; lengthOfFile-=1024) {
            send(cs,&length,sizeof(length),0);

            recv(cs,ackArray,1000,0);
            
            bzero(&buffer,length);
            inputFile.read(buffer,length);
            send(cs,buffer,length,0);

            recv(cs,ackArray,1000,0);

            send(cs,&continueGetting,sizeof(continueGetting),0);
        }
        send(cs,&lengthOfFile,sizeof(lengthOfFile),0);
        
        recv(cs,ackArray,1000,0);

        bzero(&buffer,length);
        inputFile.read(buffer,lengthOfFile);
        send(cs,buffer,lengthOfFile,0);
        
        recv(cs,ackArray,1000,0);

        continueGetting = false;
        send(cs,&continueGetting,sizeof(continueGetting),0);

    }  else {
        cout << "GET/PUT:put: FILE NOT FOUND" << endl;
        return false;
    }
    inputFile.close();
    return true;
}
bool get(MyDIR *directory, string filename, int cs){
    ofstream outputFile;
    string filePath = ((*directory).getCurrentPath() + "/" + filename);
    outputFile.open(filePath,ios_base::app);
    if (outputFile.is_open())
    {
        string content;
        bool continueGetting = true;
        do {
            int lineSize;
            recv(cs,&lineSize,sizeof(lineSize),0);

            if(lineSize == 1)
                break;
            const char *ack = "LINE_SIZE_ACK";
            send(cs,ack,sizeof(ack),0);


            //got here
            char content [lineSize];
            recv(cs,content,sizeof(content),0);

            string exactConent = content;
            exactConent = exactConent.substr(0,lineSize);
            outputFile << exactConent;

            const char *ack2 = "LINE_WRITE_ACK";
            send(cs,ack2,sizeof(ack2),0);

            recv(cs,&continueGetting,sizeof(continueGetting),0);
        } while(continueGetting);
    } else {
        cout << "GET/PUT:get: FILE NOT FOUND" << endl;
        return false;
    }
    outputFile.close();
    return true;
}
