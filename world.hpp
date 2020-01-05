#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include "graphics.h"
#include "bot.hpp"
#include <string>
#include "loc.hpp"
#include "block.hpp"
#include "timer.hpp"

#define DEFAULT_ZOOM 3
#define DEFAULT_SEED 1
#define DEFAULT_VIEWER_SPEED 10
#define DEFAULT_RESOURCE_GEN_DEPTH 8
#define DEFAULT_RESOURCE_GEN_FREQ .7
#define DEFAULT_RESOURCE_GEN_CUTOFF .82
#define STONE_CUTOFF .65
using namespace std;

class World;
class Bot;
class IntervalExecutorBus;



class Viewer {
  public:
    bool onZoom;
    double speed = DEFAULT_VIEWER_SPEED;
    double x;
    double y;
    int width = 900;
    int height = 900;
    World *world;
    void handleKeyUpEvent(SDL_Event *event);
    void handleKeyDownEvent(SDL_Event *event);
    Viewer(World *world);
};

class World
{
  public:

    int width;
    int height;
    Block ***grid;
    int zoom;
    Viewer *viewer;
    vector<Bot *> bots;
    IntervalExecutorBus *timerBus;

    World(int w, int h);

    void draw(SDL_Surface *screen);
    void setAllBlocks(BlockType type);
    void createStoneBlocks();
    void createResources(BlockType resourceType, BlockType surroundingType, double cutoff, int seed, double freq, int depth);
    void generateBots();
    void update();
    bool isCrossable(BlockType type);
    bool isCrossable(Loc loc);
};
#endif