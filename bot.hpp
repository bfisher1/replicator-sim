#ifndef BOT_H
#define BOT_H
#include "graphics.h"
#include "world.hpp"
class World;

class Bot {
  double x;
  double y;
  void update();
  void draw(SDL_Surface *screen, World *world);
};
#endif /* BOT_H */