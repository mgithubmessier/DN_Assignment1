#include <DB.h>
#include <iostream>
#include <fstream>
using namespace std;
bool authenticate(string username, string password) {
    ifstream inputFile;
    inputFile.open("users.txt");
    string line;
    //must allocate space for the input file to put characters into
    char thisUN[100], thisPW[100];
    do {
        inputFile >> thisUN;
        if(thisUN == username) {
            inputFile >> thisPW;
            if(thisPW == password) { 
                return true;
                inputFile.close();
            }
        }
    }while(getline(inputFile,line));
    inputFile.close();
    return false;
}