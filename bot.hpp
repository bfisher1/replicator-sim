#ifndef BOT_H
#define BOT_H
#include "graphics.h"
#include "world.hpp"
#include "loc.hpp"
#include "navigate.hpp"
#include <time.h> 
#define BOT_VISION_RANGE 10;
class World;
struct Block;
class Bot;
class InnerWorld;

class Bot {
  public:
    Bot(int x, int y, World *world);
    World *world;
    double x;
    double y;
    double dir = 0;
    double vel = 0;
    int visionRange;
    void update();
    void draw(SDL_Surface *screen);
    void updateBlocksInVision();
    vector<Loc> *createRoute(Loc target);

    // mind
    InnerWorld *innerWorld;
};
#endif /* BOT_H */