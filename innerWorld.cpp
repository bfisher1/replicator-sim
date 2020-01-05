#include "innerWorld.hpp"

InnerWorld::InnerWorld(World *world, Bot *b) {
  map = new World(world->width, world->height);
  map->viewer = world->viewer;
  focus = NULL;
  targetPath = NULL;
  
  map->setAllBlocks(BlockType::unknown);
  self = b;
}

void InnerWorld::draw(SDL_Surface *screen) {
  map->draw(screen);
  map->bots = vector<Bot*>();
  map->bots.push_back(self);
  // drawRect(screen, self->x * map->zoom, self->y * map->zoom, map->zoom, map->zoom, (Color) {255, 0, 0});
  // drawRect(screen, target.x * map->zoom, target.y * map->zoom, map->zoom, map->zoom, (Color) {0, 0, 255});
  // for(int i = 0; i < targetPath->size(); i++) {
  //   Loc loc = (*targetPath)[i];
  //   drawRect(screen, loc.x * map->zoom, loc.y * map->zoom, map->zoom, map->zoom, (Color) {0, 0, 255});
  // }
}

Loc InnerWorld::botLoc() {
  return Loc(self->x, self->y);
}

bool InnerWorld::isCrossable(Loc loc) {
  return map->isCrossable(map->grid[(int) loc.x][(int) loc.y]->type);
}