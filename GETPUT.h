#include <string>
#include <Command.h>
using namespace std;
//void writeStringToFile(MyDIR *directory, string filename, string content);
//string readFileToString(MyDIR *directory, string filename);
bool get(MyDIR *directory, string filename, int cs);
bool put(MyDIR *directory, string filename, int cs);

const string FNF = "!!!FILE NOT FOUND!!!";