#include "bot.hpp"

void Bot::update() {
  x += .1;
  updateBlocksInVision();
  createRoute(Loc(54, 54));
  //world->grid[(int) x][(int) y].type = BlockType::water;
}
void Bot::draw(SDL_Surface *screen) {
  drawRect(screen, (x - world->viewer->x) * world->zoom, (y - world->viewer->y) * world->zoom, world->zoom, world->zoom, (Color) {50, 50, 50});
  innerWorld->draw(screen);
}

void Bot::updateBlocksInVision() {
  for(int i = x - visionRange / 2; i < x + visionRange / 2; i++) {
    for(int j = y - visionRange / 2; j < y + visionRange / 2; j++) {
      innerWorld->map->grid[i][j].type = world->grid[i][j].type;
    }
  }
}

Bot::Bot(int sx, int sy, World *w) {
  x = sx;
  y = sy;
  world = w;
  visionRange = BOT_VISION_RANGE;
  innerWorld = new InnerWorld(world, this);
}

void Bot::createRoute(Loc target) {
  createNaiveDjikstraRoute(innerWorld, target);
}