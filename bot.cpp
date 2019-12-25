#include "bot.hpp"
#include <math.h>
#include "util.hpp"
#include <iostream>
#define TURN_SENSITIVITY 1.62

void wanderUpdateMove(Bot *bot) {
  bot->vel = .2;
  bot->dir += TURN_SENSITIVITY * (randNorm() - .5);
}

void move(Bot *bot) {
  bot->x += bot->vel * cos(bot->dir);
  bot->y += bot->vel * sin(bot->dir);
}

void Bot::update() {
  time_t now;
  time(&now);
  if(now > innerWorld->lastUpdate) {
    innerWorld->lastUpdate = now;
    wanderUpdateMove(this);
  }
  move(this);
  updateBlocksInVision();
  innerWorld->target = Loc(80, 120);
  innerWorld->targetPath = createRoute(innerWorld->target);
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

vector<Loc> *Bot::createRoute(Loc target) {
  return createNaiveDjikstraRoute(innerWorld, target);
}

/*
wander()
search() = goto stones, then wander and mine
*/