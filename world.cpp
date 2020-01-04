#include "world.hpp"
#include "perlin.h"
#include "util.hpp"
#include "anim.hpp"
#include <iostream>

// todo, centralize this here
#define MIN_X 0
#define MIN_Y 0
#define WIDTH 640
#define HEIGHT 480
#define MAX_ZOOM 20
#define MIN_ZOOM 1
#define TREE_BLOCKS_HEIGHT 1

int seed = DEFAULT_SEED;
int getNextSeed() {
  seed += 5;
  return seed;
}

string blockAnimName(Block *block) {
  switch(block->type) {
    case BlockType::stone:
      return "stone.png";
    case BlockType::water:
      return "water-1.png";
    case BlockType::air:
      return "grass.png";
    case BlockType::coal:
      return "coal.png";
    case BlockType::copper:
      return "copper.png";
    case BlockType::sand:
      return "sand.png";
    case BlockType::tree:
      return "grass.png";
    case BlockType::zinc:
      return "zinc.png";
    case BlockType::iron:
      return "iron.png";
    case BlockType::nickel:
      return "nickel.png";
    case BlockType::silicon:
      return "silicon.png";
    case BlockType::unknown:
       return "black.png";
    default:
      return "black.png";
  }
}



bool World::isCrossable(BlockType type) {
  switch(type) {
    case BlockType::air:
    case BlockType::sand:
      return true;
    default:
      return false;
  }
}

bool World::isCrossable(Loc loc) {
  return isCrossable(grid[(int) loc.x][(int) loc.y]->type);
}


World::World(int w, int h) {
  width = w;
  height = h;
  zoom = DEFAULT_ZOOM;
  grid = new Block**[width];
  for(int i = 0; i < width; i++)
    grid[i] = new Block*[height];
  
  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {
      grid[i][j] = new AirBlock();
    }
  }
  
  setAllBlocks(BlockType::air);
  createStoneBlocks();
  createResources(BlockType::coal, BlockType::stone, DEFAULT_RESOURCE_GEN_CUTOFF, getNextSeed(), DEFAULT_RESOURCE_GEN_FREQ, DEFAULT_RESOURCE_GEN_DEPTH);
  createResources(BlockType::copper, BlockType::stone, DEFAULT_RESOURCE_GEN_CUTOFF, getNextSeed(), DEFAULT_RESOURCE_GEN_FREQ, DEFAULT_RESOURCE_GEN_DEPTH);
  createResources(BlockType::zinc, BlockType::stone, DEFAULT_RESOURCE_GEN_CUTOFF, getNextSeed(), DEFAULT_RESOURCE_GEN_FREQ, DEFAULT_RESOURCE_GEN_DEPTH);
  createResources(BlockType::zinc, BlockType::stone, DEFAULT_RESOURCE_GEN_CUTOFF, getNextSeed(), DEFAULT_RESOURCE_GEN_FREQ, DEFAULT_RESOURCE_GEN_DEPTH);
  createResources(BlockType::silicon, BlockType::stone, DEFAULT_RESOURCE_GEN_CUTOFF, getNextSeed(), DEFAULT_RESOURCE_GEN_FREQ, DEFAULT_RESOURCE_GEN_DEPTH);
  createResources(BlockType::nickel, BlockType::stone, DEFAULT_RESOURCE_GEN_CUTOFF, getNextSeed(), DEFAULT_RESOURCE_GEN_FREQ, DEFAULT_RESOURCE_GEN_DEPTH);
  
  // sand and water
  int beachSeed = getNextSeed();
  createResources(BlockType::sand, BlockType::air, .50, beachSeed, .08, 4);
  createResources(BlockType::water, BlockType::sand, .65, beachSeed, .08, 4);
  // trees
  createResources(BlockType::tree, BlockType::air, .65, getNextSeed(), .8, 4);

  // setting up viewer for user to view world
  viewer = new Viewer(this);

  // set initial block animations
  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {
      grid[i][j]->anim = getAnim(blockAnimName(grid[i][j]));
    }
  }
  
}

void World::createStoneBlocks() {
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      double height = Perlin_Get2d((double) i, (double) j, .04, 4, DEFAULT_SEED);
      if(height > STONE_CUTOFF) {
        grid[i][j] = new StoneBlock();
      }
    }
  }
}

void World::createResources(BlockType resourceType, BlockType surroundingType, double cutoff, int seed, double freq, int depth) {
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      double height = Perlin_Get2d((double) i, (double) j, freq, depth, seed);
      if(height > cutoff && grid[i][j]->type == surroundingType) {
        grid[i][j] = newblockFromType(resourceType);
      }
    }
  }
}

void World::setAllBlocks(BlockType type) {
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      grid[i][j] = newblockFromType(type);
    }
  }
}

void World::generateBots() {
  bots.push_back(new Bot(9, 13, this));
}

void World::draw(SDL_Surface *screen) {

  int scale = 32;

  // WIDTH & HEIGHT = world width and height

  // viewer width & height = pixels that viewer can see

  vector<Loc> treeCoords = vector<Loc>();

  for(int i = int(viewer->x); (i - viewer->x) * scale < viewer->width && i < width; i++) {
    for(int j = int(viewer->y); (j - viewer->y) * scale < viewer->height && j < height; j++) {
      grid[i][j]->draw((i - viewer->x) * scale, (j - viewer->y) * scale, 1);
      if(grid[i][j]->type == BlockType::tree) {
        treeCoords.push_back(Loc(i, j));
      }
    }
  }
  // todo move this to block draw
  //draw trees
  for(int i = 0; i < treeCoords.size(); i++) {
    drawStillFrame(getAnim("tree-big.png"), (treeCoords[i].x - viewer->x) * scale, (treeCoords[i].y - TREE_BLOCKS_HEIGHT - viewer->y) * scale, 0, false);
  }

  //draw bots
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