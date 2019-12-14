#include "world.hpp"
#include "perlin.h"

// todo, centralize this here
#define WIDTH 640
#define HEIGHT 480

int seed = DEFAULT_SEED;
int getNextSeed() {
  seed += 5;
  return seed;
}

Color blockColor(Block block) {
  switch(block.type) {
    case BlockType::stone:
      return (Color) {136, 136, 136};
    case BlockType::water:
      return (Color) {0, 0, 255};
    case BlockType::air:
      return (Color) {0, 200, 0};
    case BlockType::coal:
      return (Color) {0, 0, 0};
    case BlockType::copper:
      return (Color) {255, 128, 0};
    case BlockType::sand:
      return (Color) {255, 255, 223};
    default:
      return (Color) {0, 0, 0};
  }
}

World::World(int w, int h) {
  width = w;
  height = h;
  zoom = DEFAULT_ZOOM;
  grid = new Block*[height];
  for(int i = 0; i < height; ++i)
    grid[i] = new Block[width];
  
  setAllBlocks(BlockType::air);
  createStoneBlocks();
  createResources(BlockType::coal, BlockType::stone, .8, getNextSeed(), .7, 4);
  createResources(BlockType::copper, BlockType::stone, .7, getNextSeed(), .7, 4);
  // sand and water
  int beachSeed = getNextSeed();
  createResources(BlockType::sand, BlockType::air, .50, beachSeed, .08, 4);
  createResources(BlockType::water, BlockType::sand, .65, beachSeed, .08, 4);
  
  
}

void World::createStoneBlocks() {
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      double height = Perlin_Get2d((double) i, (double) j, .04, 4, DEFAULT_SEED);
      if(height > .55) {
        grid[i][j].type = BlockType::stone;
      }
    }
  }
}

void World::createResources(BlockType resourceType, BlockType surroundingType, double cutoff, int seed, double freq, int depth) {
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      double height = Perlin_Get2d((double) i, (double) j, freq, depth, DEFAULT_SEED);
      if(height > cutoff && grid[i][j].type == surroundingType) {
        grid[i][j].type = resourceType;
      }
    }
  }
}

void World::setAllBlocks(BlockType type) {
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      grid[i][j].type = type;
    }
  }
}

void World::draw(SDL_Surface *screen) {
  Color blue = {0, 0, 255};
  Color red = {255, 0, 0};
  Color green = {255, 0, 0};
  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {
      drawRect(screen, i * zoom, j * zoom, zoom, zoom, blockColor(grid[i][j]));
    }
  }
}