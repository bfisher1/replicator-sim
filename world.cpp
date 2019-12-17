#include "world.hpp"
#include "perlin.h"
#include "util.h"

// todo, centralize this here
#define MIN_X 0
#define MIN_Y 0
#define WIDTH 640
#define HEIGHT 480
#define MAX_ZOOM 20
#define MIN_ZOOM 1

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
    case BlockType::unknown:
      return (Color) {0, 0, 0};
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
  
  // setting up viewer for user to view world
  viewer = new Viewer(this);
  
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

void World::generateBots() {
  bots.push_back(new Bot(50, 50, this));
}

void World::draw(SDL_Surface *screen) {
  Color blue = {0, 0, 255};
  Color red = {255, 0, 0};
  Color green = {255, 0, 0};
  // drawing the world
  for(int i = int(viewer->x); i < width; i++) {
    if(i > viewer->width) {
      break;
    }
    for(int j = int(viewer->y); j < height; j++) {
      if(j > viewer->height) {
        break;
      }
      drawRect(screen, (i - viewer->x) * zoom, (j - viewer->y) * zoom, zoom, zoom, blockColor(grid[i][j]));
    }
  }
  for(int i = 0; i < bots.size(); i++) {
    bots[i]->draw(screen);
  }
}

void World::update() {
  for(int i = 0; i < bots.size(); i++) {
    bots[i]->update();
  }
}

Viewer::Viewer(World *w) {
  world = w;
  x = 0;
  y = 0;
}

void Viewer::handleKeyUpEvent(SDL_Event *event) {
  switch(event->key.keysym.sym) {
    case SDLK_LEFTBRACKET:
      if(!onZoom) {
        onZoom = true;
        if(world->zoom > MIN_ZOOM) {
          world->zoom -= 1;
        }
      }
      break;
    case SDLK_RIGHTBRACKET:
      if(!onZoom) {
        onZoom = true;
        if(world->zoom < MAX_ZOOM) {
          world->zoom += 1;
        }
      }
    default:
      break;
  }
}
void Viewer::handleKeyDownEvent(SDL_Event *event) {
  switch(event->key.keysym.sym) {
    case SDLK_LEFTBRACKET:
    case SDLK_RIGHTBRACKET:
      onZoom = false;
      break;
    case SDLK_UP:
      if(y > MIN_Y) {
        y -= speed;
      }
      break;
    case SDLK_DOWN:
      if(y < HEIGHT) {
        y += speed;
      }
      break;
    case SDLK_LEFT:
      if(x > MIN_X) {
        x -= speed;
      }
      break;
    case SDLK_RIGHT:
      if(x < WIDTH) {
        x += speed;
      }
      break;
    default:
      break;
  }
}