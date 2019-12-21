#include "loc.hpp"
#include "util.h"
#include <stdio.h>
#include <iostream>
#include <string>

Loc::Loc(const Loc &other) {
  x = other.x;
  y = other.y;
}
Loc::Loc(double x1, double y1) {
  x = x1;
  y = y1;
}

Loc::Loc() {}

void Loc::setBoundingBox(Loc l1, Loc l2, Loc& topLeft, Loc& bottomRight) {
  topLeft.x = min(l1.x, l2.x);
  bottomRight.x = max(l1.x, l2.x);
  topLeft.y = min(l1.y, l2.y);
  bottomRight.y = max(l1.y, l2.y);
}

bool Loc::sameLoc(Loc loc1, Loc loc2) {
  return ((int) loc1.x) == ((int) loc2.x) && ((int) loc1.y) == ((int) loc2.y);
}

string Loc::toString() {
  string str;
  char locBuffer[25];
  sprintf(locBuffer, "(%.2f, %.2f)", x, y);
  return string(locBuffer);
}

Loc Loc::plus(Loc loc) {
  return Loc(loc.x + x, loc.y + y);
}