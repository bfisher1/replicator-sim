#include "bot.hpp"

void Bot::update() {
  x += .1;
}
void Bot::draw(SDL_Surface *screen, World *world) {
  drawRect(screen, (x - world->viewer->x) * world->zoom, (y - world->viewer->y) * world->zoom, world->zoom, world->zoom, (Color) {50, 50, 50});
}

Bot::Bot(int sx, int sy) {
  x = sx;
  y = sy;
}