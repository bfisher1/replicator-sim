#include "innerWorld.hpp"

InnerWorld::InnerWorld(World *world, Bot *b) {
  map = new World(world->width, world->height);
  map->viewer = new Viewer(map);
  map->viewer->x = 0;
  map->viewer->y = 0;
  map->viewer->width = 200;
  map->viewer->height = 200;
  map->zoom = 3;
  
  map->setAllBlocks(BlockType::unknown);
  self = b;
}

void InnerWorld::draw(SDL_Surface *screen) {
  map->draw(screen);
  drawRect(screen, self->x * map->zoom, self->y * map->zoom, map->zoom, map->zoom, (Color) {255, 0, 0});
}