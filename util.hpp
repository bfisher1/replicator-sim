#ifndef UTIL_H
#define UTIL_H
#include <vector>
#include <string>
using namespace std;
double max(double x, double y);

double min(double x, double y);
double randNorm();
vector<string> *getFileNamesInPath(string dir);
bool stringEndsWith(string str, string end);
#endif