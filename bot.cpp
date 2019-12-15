#include "bot.hpp"

void Bot::update() {
  x += .1;
}
void Bot::draw(SDL_Surface *screen, World *world) {
  drawRect(screen, x, y, world->zoom, world->zoom, (Color) {50, 50, 50});
}
