#include "bot.hpp"

void Bot::update() {
  x += .1;
  world->grid[(int) x][(int) y].type = BlockType::water;
}
void Bot::draw(SDL_Surface *screen) {
  drawRect(screen, (x - world->viewer->x) * world->zoom, (y - world->viewer->y) * world->zoom, world->zoom, world->zoom, (Color) {50, 50, 50});
}

Bot::Bot(int sx, int sy, World *w) {
  x = sx;
  y = sy;
  world = w;
}