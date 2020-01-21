#include "direction.hpp"
#include <math.h>

Direction angleToDir(double angle) {
  // radians to degrees
  int deg = angle * 180.0 / M_PI;
  // normalize to 0-360 interval
  deg %= 360;
  if(deg < 90) {
    return Direction::up;
  }
  else if(deg < 180) {
    return Direction::right;
  }
  else if(deg < 270) {
    return Direction::down;
  }
  else {
    return Direction::left;
  }
}

Loc addVectorToLoc(Loc origin, double dist, Direction dir) {
  switch(dir) {
    case Direction::up:
      return origin.plus(Loc(0, -dist));
    case Direction::down:
      return origin.plus(Loc(0, dist));
    case Direction::left:
      return origin.plus(Loc(-dist, 0));
    case Direction::right:
      return origin.plus(Loc(dist, 0));
  }
}