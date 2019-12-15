#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include "graphics.h"
#include "bot.hpp"
#define DEFAULT_ZOOM 3
#define DEFAULT_SEED 1
#define DEFAULT_VIEWER_SPEED 10

using namespace std;

class World;
class Bot;

enum BlockType {
  stone,
  water,
  coal,
  copper,
  air,
  sand
};

struct Block {
  BlockType type;
};

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
    Block **grid;
    int zoom;
    Viewer *viewer;
    vector<Bot *> bots;

    World(int w, int h);

    void draw(SDL_Surface *screen);
    void setAllBlocks(BlockType type);
    void createStoneBlocks();
    void createResources(BlockType resourceType, BlockType surroundingType, double cutoff, int seed, double freq, int depth);
    void generateBots();
    void update();
};
#endif