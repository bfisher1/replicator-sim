#include "graphics.h"
#define DEFAULT_ZOOM 3
#define DEFAULT_SEED 1

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

class World
{
  public:

    int width;
    int height;
    Block **grid;
    int zoom;

    World(int w, int h);

    void draw(SDL_Surface *screen);
    void setAllBlocks(BlockType type);
    void createStoneBlocks();
    void createResources(BlockType resourceType, BlockType surroundingType, double cutoff, int seed, double freq, int depth);
};