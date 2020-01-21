#include <stdlib.h> 
#include "util.hpp"
#include <stdio.h> 
#include <dirent.h> 
#include <string>

double max(double x, double y) {
  if(x > y)
    return x;
  return y;
}

double min(double x, double y) {
  if(x < y)
    return x;
  return y;
}

double randNorm() {
  return (float) rand() / (float) RAND_MAX;
}

double randRange(double start, double end) {
  return start + (end - start) * randNorm();
}

// using code from: https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/
vector<string> *getFileNamesInPath(string dir) {
  vector<string> *fileNames = new vector<string>();
  struct dirent *de;  // Pointer for directory entry 
  // opendir() returns a pointer of DIR type.  
  DIR *dr = opendir(dir.data()); 

  if (dr == NULL)  // opendir returns NULL if couldn't open directory 
  { 
    return fileNames; 
  } 

  // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
  // for readdir() 
  while ((de = readdir(dr)) != NULL) 
    fileNames->push_back(de->d_name); 

  closedir(dr);
  return fileNames;
}


bool stringEndsWith(string str, string end) {
  // if the line ends with end
  if(str.size() - end.length() == str.rfind(end.data(), str.size() - end.length())) {
    return true;
  }
  return false;
}
