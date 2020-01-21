#include "innerWorld.hpp"
#include "direction.hpp"
#include <unistd.h>

// used for comparing locs, done on a distance basis
static bool operator<(const Loc& l1, const Loc& l2) {
  return l1.x * l1.x + l1.y * l1.y < l2.x * l2.x + l2.y * l2.y;
}

InnerWorld::InnerWorld(World *world, Bot *b) {
  map = new World(world->width, world->height);
  map->viewer = world->viewer;
  focus = NULL;
  targetPath = NULL;
  
  map->setAllBlocks(BlockType::unknown);
  self = b;
  explore = new Explore(Direction::down);
  selected = new set<Loc>();

  // add self to vector of bots in inner world
  map->bots = vector<Bot*>();
  map->bots.push_back(self);
  targetPath = new vector<Loc>();
  targetPathIterator = NULL;
  plots = new vector<Loc>();
  highLightPlots = new set<int>();
}

void InnerWorld::draw(SDL_Surface *screen) {
  cout << "Is null " << endl << (self == NULL) << "..." << endl;
  map->draw(screen);

  // drawRect(screen, self->x * map->zoom, self->y * map->zoom, map->zoom, map->zoom, (Color) {255, 0, 0});
  // drawRect(screen, target.x * map->zoom, target.y * map->zoom, map->zoom, map->zoom, (Color) {0, 0, 255});
  // for(int i = 0; i < targetPath->size(); i++) {
  //   Loc loc = (*targetPath)[i];
  //   drawRect(screen, loc.x * map->zoom, loc.y * map->zoom, map->zoom, map->zoom, (Color) {0, 0, 255});
  // }
  int scale = 32;

  for(auto it = selected->begin(); it != selected->end(); it++) {
    int x = it->x;
    int y = it->y;
    drawStillFrame(getAnim("select-red.png"), (x - map->viewer->x) * scale, (y - map->viewer->y) * scale, 0, false);
  }
  int i = 0;
  for(auto it = plots->begin(); it != plots->end(); it++) {
    double x = it->x;
    double y = it->y;
    drawStillFrame(getAnim("plot-red.png"), (int) ((x - map->viewer->x) * scale), (int) ((y - map->viewer->y) * scale), 0, false);
    for(auto it2 = highLightPlots->begin(); it2 != highLightPlots->end(); it2++) {
      if(*it2 == i) {
        drawStillFrame(getAnim("plot-blue.png"), (int) ((x - map->viewer->x) * scale), (int) ((y - map->viewer->y) * scale), 0, false);
        //sleep(1);
      }
    }
    i++;
  }
}

Loc InnerWorld::botLoc() {
  return Loc(self->x, self->y);
}

bool InnerWorld::isCrossable(Loc loc) {
  return map->grid[(int) loc.x][(int) loc.y]->crossable;
}


void InnerWorld::select(Loc loc) {
  selected->insert(loc);
}

void InnerWorld::deselect(Loc loc) {
  if(isSelected(loc)) {
    selected->erase(loc);
  }
}

void InnerWorld::deselectAll() {
  selected = new set<Loc>();
}

void InnerWorld::toggleTargetPathSelect(bool val) {
  for(int i = 0; i < targetPath->size(); i++) {
    Loc loc = (*targetPath)[i];
    if(val) {
      select(loc);
    } else {
      deselect(loc);
    }
  }
}

bool InnerWorld::isSelected(Loc loc) {
  return selected->count(loc) != 0;
}