#ifndef INNER_WORLD_H
#define INNER_WORLD_H
#include "world.hpp"
#include "bot.hpp"
#include "loc.hpp"
#include "innerWorld.hpp"
#include "explore.hpp"
#include "path.hpp"
#include <set>

class World;
class Bot;
class Explore;
class PathIterator;

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
    Explore *explore;
    set<Loc> *selected;
    // like selected, but clear out every update
    vector<Loc> *plots;
    set<int> *highLightPlots;
    void select(Loc loc);
    void deselect(Loc loc);
    void deselectAll();
    bool isSelected(Loc loc);
    void toggleTargetPathSelect(bool val);
    PathIterator *targetPathIterator;
    // lists of resources
};

#endif