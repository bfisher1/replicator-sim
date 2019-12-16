#ifndef BOT_H
#define BOT_H
#include "graphics.h"
#include "world.hpp"
class World;

class Bot {
  public:
    Bot(int x, int y, World *world);
    World *world;
    double x;
    double y;
    void update();
    void draw(SDL_Surface *screen);
};
#endif /* BOT_H */