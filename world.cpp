#include "world.hpp"
#include "perlin.h"
#include "util.hpp"
#include "anim.hpp"
#include <iostream>
#include "loc.hpp"
#include <stdio.h>

// todo, centralize this here
#define MIN_X 0
#define MIN_Y 0
#define WIDTH 640
#define HEIGHT 480
#define MAX_ZOOM 20
#define MIN_ZOOM 1
#define TREE_AGE_CHANCE .15
// todo make this 60 or 120
#define TREE_AGE_INTERVAL 6
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
    // case BlockType::bedrock:
    //   return "bedrock.png";
    case BlockType::unknown:
      return "unknown.png";
    default:
      cout << "Truly unknown block: " << block->name << endl;
      exit(0);
      return "black.png";
  }
}

DynamicResource::DynamicResource() {
  //initialize the lock
  if(pthread_mutex_init(&rsrcLock, NULL) != 0) {
    printf("Couldn't create monitor.");
    exit(0);
  }
  locked = false;
}

void DynamicResource::lock() {
  pthread_mutex_lock(&rsrcLock);
  locked = true;
}

void DynamicResource::unlock() {
  pthread_mutex_unlock(&rsrcLock);
  locked = false;
}



// used for comparing locs, done on a distance basis
static bool operator<(const Loc& l1, const Loc& l2) {
  return l1.x * l1.x + l1.y * l1.y < l2.x * l2.x + l2.y * l2.y;
}

// bool World::select(Loc loc, bool val) {
//   bool prev = grid[(int) loc.x][(int) loc.y]->selected;
//   grid[(int) loc.x][(int) loc.y]->selected = val;
//   return prev;
// }

bool World::isCrossable(Loc loc) {
  return grid[(int) loc.x][(int) loc.y]->crossable;
}


void updateTrees(World *world) {
  for(auto it = world->trees->begin(); it != world->trees->end(); it++) {
    if(randNorm() <= TREE_AGE_CHANCE) {
      it->second->ageTree();
    }
  }
}

void setBlockAnims(World *world) {
  //cout << "set block anims " << endl;
  for(int i = 0; i < world->width; i++) {
    for(int j = 0; j < world->height; j++) {
      //cout << "Type " << world->grid[i][j]->name << endl;
      world->grid[i][j]->anim = getAnim(blockAnimName(world->grid[i][j]));
    }
  }
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
  trees = new map<Loc, TreeBlock *>();
  vector<Loc> *treeLocs = createResources(BlockType::tree, BlockType::air, .65, getNextSeed(), .8, 4);
  //map<Loc, TreeBlock *> trees;
  for(int i = 0; i < treeLocs->size(); i++) {
    Loc loc = (*treeLocs)[i];
    (*trees)[loc] = (TreeBlock *) grid[(int) loc.x][(int) loc.y];
  }

  // setting up viewer for user to view world
  viewer = new Viewer(this);

  //interval events
  timerBus = new IntervalExecutorBus(this);
  //timerBus->addIntervalExecutor(TREE_AGE_INTERVAL, updateTrees);


  // set initial block animations
  setBlockAnims(this);
  
  
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

/*
Returns list of locations of the resource added.
*/
vector<Loc> *World::createResources(BlockType resourceType, BlockType surroundingType, double cutoff, int seed, double freq, int depth) {
  vector<Loc> *locs = new vector<Loc>();
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      double height = Perlin_Get2d((double) i, (double) j, freq, depth, seed);
      if(height > cutoff && grid[i][j]->type == surroundingType) {
        Block *oldBlock = grid[i][j];
        Block *newBlock = newblockFromType(resourceType);
        grid[i][j] = newBlock;
        newBlock->replacementOf(oldBlock);
        locs->push_back(Loc(i, j));
      }
    }
  }
  return locs;
}

void World::setAllBlocks(BlockType type) {
  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      grid[i][j] = newblockFromType(type);
      // cout << "new block from type " << grid[i][j]->name << " --- " << (type == BlockType::bedrock)  << endl;
    }
  }
  setBlockAnims(this);
}

void World::generateBots() {
  bots.push_back(new Bot(9, 13, this));
}

void World::draw(SDL_Surface *screen) {

  int scale = 32;

  // WIDTH & HEIGHT = world width and height

  // viewer width & height = pixels that viewer can see

  vector<Loc> treeCoords = vector<Loc>();

  // cout << "-------------" << endl;

  for(int i = int(viewer->x); (i - viewer->x) * scale < viewer->width && i < width; i++) {
    for(int j = int(viewer->y); (j - viewer->y) * scale < viewer->height && j < height; j++) {

      // cout << "drawing world " << grid[i][j]->name << endl;
      // if(grid[i][j]->selected) {
      //   cout << "selected" << endl;
      // }

      
      if(grid[i][j]->type == BlockType::tree) {
        treeCoords.push_back(Loc(i, j));
      } else {
        grid[i][j]->draw((i - viewer->x) * scale, (j - viewer->y) * scale, 1);
      }
    }
  }

  //TODO DRAW BASED OFF OF HEIGHTS in z axis

  //draw trees
  for(int i = 0; i < treeCoords.size(); i++) {
    TreeBlock *treeBlock = (TreeBlock *) grid[(int) treeCoords[i].x][(int) treeCoords[i].y];
    treeBlock->drawTree((treeCoords[i].x - viewer->x) * scale, (treeCoords[i].y - TREE_BLOCKS_HEIGHT - viewer->y) * scale, (treeCoords[i].y - viewer->y) * scale, 1);
  }

  //draw bots
  for(int i = 0; i < bots.size(); i++) {
    bots[i]->draw(screen);
  }

}

void World::update() {
  for(int i = 0; i < bots.size(); i++) {
    if(!bots[i]->started) {
      bots[i]->start();
    }
    //bots[i]->update();
  }
  timerBus->checkAndRunExecutors();
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