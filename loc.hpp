#ifndef LOC_H
#define LOC_H
class Loc {
  public:
    double x;
    double y;
    Loc(double x, double y);
    Loc(const Loc &other);
};
#endif