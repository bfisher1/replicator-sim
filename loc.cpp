#include "loc.hpp"

Loc::Loc(const Loc &other) {
  x = other.x;
  y = other.y;
}
Loc::Loc(double x1, double y1) {
  x = x1;
  y = y1;
}