#include <string>
using namespace std;
struct Message {
	int cmd;
	string parameter;
};
Message parseMessage(string input);
void clientCommLoop(int cs);
void serverCommLoop(int cs, int s);


