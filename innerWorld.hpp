#ifndef INNER_WORLD_H
#define INNER_WORLD_H
#include "world.hpp"
#include "bot.hpp"
#include "loc.hpp"
#include "innerWorld.hpp"
class World;
class Bot;

// bot's representation of the world
class InnerWorld {
  public:
    World *map;
    Bot *self;
    InnerWorld(World *world, Bot *bot);
    void draw(SDL_Surface *screen);
    Loc botLoc();
    bool isCrossable(Loc loc);
    Loc target;
    Loc *focus;
    vector<Loc> *targetPath;
    time_t lastUpdate;
    // lists of resources
};

#endif