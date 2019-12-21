#ifndef LOC_H
#define LOC_H
#include <string>

using namespace std;

class Loc {
  public:
    double x;
    double y;
    Loc(double x, double y);
    Loc(const Loc &other);
    Loc();
    static void setBoundingBox(Loc src, Loc target, Loc& topLeft, Loc& bottomRight);
    static bool sameLoc(Loc loc1, Loc loc2);
    string toString();
    Loc plus(Loc loc);
};
#endif