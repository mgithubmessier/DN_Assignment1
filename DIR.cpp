#include <DIR.h>
#include <dirent.h>
#ifdef WINDOWS
#include <direct.h>
    #define GetCurrentDir _getcwd
#else
#include <unistd.h>
    #define GetCurrentDir getcwd
 #endif
#include <iostream>
MyDIR::MyDIR() {
  currentPath = getInitialPath();
  bool dirChanged = changeToDIR(".");
}
string MyDIR::getCurrentDIR() {
  return currentDIR;
}
string MyDIR::getCurrentPath() {
  return currentPath;
}
string MyDIR::getInitialPath() {
   char cCurrentPath[FILENAME_MAX];
  if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
      return "SOMETHING WENT WRONG WITH THE DIR";
    }
  return cCurrentPath;
}
bool MyDIR::changeToDIR(string newPath) {
  string lastDIR = currentDIR;
  string lastPath = currentPath;
  if(newPath == "..") {
    int iLastSlash = currentPath.find_last_of("/");
    currentPath.replace(iLastSlash,currentPath.length() - iLastSlash,"");
  } else if(newPath != ".") {
    currentPath = currentPath + "/" + newPath;
  }
  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir (currentPath.c_str())) != NULL) {
    /* print all the files and directories within directory */
    currentDIR = "";
    while ((ent = readdir (dir)) != NULL) {
      currentDIR += ent->d_name;
      currentDIR += "\n";
    }
    closedir (dir);
    return true;
  } else {
    cout << "DIR:changeToDIR: cout not open directory" << endl;
    currentDIR = lastDIR;
    currentPath = lastPath;
    return false;
  }
}
