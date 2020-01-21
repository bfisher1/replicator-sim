#ifndef BOT_H
#define BOT_H
#include "graphics.h"
#include "world.hpp"
#include "loc.hpp"
#include "navigate.hpp"
#include <time.h>
#include <math.h>
#include "mob.hpp"
#include <pthread.h>


#define BOT_VISION_RANGE 10;
class World;
struct Block;
class Bot;
class InnerWorld;

class Mob {
  public:
    //static vector<Mob *> *mobs;
    pthread_t thread;
    World *world;
    Mob(World *world);
    bool started;
    void start();
    
};


class Bot : public Mob {
  public:
    Bot(int x, int y, World *world);
    double x;
    double y;
    double dir = M_PI/2;
    double vel = 0;
    int visionRange;
    void update();
    void draw(SDL_Surface *screen);
    void updateBlocksInVision();
    vector<Loc> *createRoute(Loc target);
    void face(Loc loc);
    int width = 1;
    int length = 1;
    bool traverseTargetPath();
    bool pressed;
    void exec();

    // mind
    InnerWorld *innerWorld;
};
#endif /* BOT_H */