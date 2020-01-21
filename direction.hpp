#ifndef DIRECTION_H
#define DIRECTION_H
#define DIRECTION_NUM 4
#include "loc.hpp"

enum Direction {
  up = 0,
  down = 1,
  left = 2,
  right = 3
};

Direction angleToDir(double angle);

Loc addVectorToLoc(Loc origin, double dist, Direction dir);

#endif /* DIRECTION_H */
