#include <string>
using namespace std;
class MyDIR {
	private:
		string currentPath;
		string currentDIR;
	public:
		bool changeToDIR(string currentPath);
		string getInitialPath();
		string getCurrentPath();
		string getCurrentDIR();
		void instantiateDIR();
		MyDIR();
};

