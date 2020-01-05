#include "bot.hpp"
#include "util.hpp"
#include <iostream>
#include "anim.hpp"
#include "direction.hpp"

#define TURN_SENSITIVITY 1.62

void wanderUpdateMove(Bot *bot) {
  //bot->vel = .04;
  bot->dir += TURN_SENSITIVITY * (randNorm() - .5);
  bot->vel = .0514;
  //bot->dir = -(3.14 / 2 - .6);
}

void move(Bot *bot) {
  double dx = bot->vel * cos(bot->dir);
  double dy = bot->vel * sin(bot->dir);
  double newX = bot->x + dx;
  double newY = bot->y + dy;
  
  // need to get blocks that the new coords intersect with
  // to see if the robot intersects with a block, just check the blocks the corners are in

  // we can just cast the coords to ints to get the block index
  int left = newX - bot->width / 2;
  int right = newX + bot->width / 2 + 1;
  int top = newY - bot->length / 2;
  int bottom = newY + bot->length / 2 + 1;

  // if any of the blocks aren't crossable, don't allow move
  if(bot->world->isCrossable(Loc(left,top)) && bot->world->isCrossable(Loc(right,top))
      && bot->world->isCrossable(Loc(left,bottom)) && bot->world->isCrossable(Loc(right,bottom))) {
    bot->x = newX;
    bot->y = newY;
  }
  else {
    bot->dir += M_PI;
  }
  // cout << "top left " << bot->world->isCrossable(Loc(left,top)) << bot->world->grid[left][top]->name << endl;
  // cout << "top right " << bot->world->isCrossable(Loc(right,top)) << bot->world->grid[right][top]->name << endl;
  // cout << "bottom left " << bot->world->isCrossable(Loc(left,bottom)) << bot->world->grid[left][bottom]->name << endl;
  // cout << "bottom right " << bot->world->isCrossable(Loc(right,bottom)) << bot->world->grid[right][bottom]->name << endl;
  
  // cout << "----" << endl;
  bot->innerWorld->targetPath = new vector<Loc>();
  bot->innerWorld->targetPath->push_back(Loc(left,top));
  bot->innerWorld->targetPath->push_back(Loc(right,top));
  bot->innerWorld->targetPath->push_back(Loc(left,bottom));
  bot->innerWorld->targetPath->push_back(Loc(right,bottom));
  
  //bot->innerWorld->focus = new Loc(right, bottom);  
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
  return;
  innerWorld->target = Loc(80, 120);
  innerWorld->targetPath = createRoute(innerWorld->target);
  //world->grid[(int) x][(int) y]->type = BlockType::water;
}
void Bot::draw(SDL_Surface *screen) {
  int scale = 32;
  drawStillFrameRot(getAnim("bot.png"), (x - world->viewer->x) * scale, (y - world->viewer->y) * scale, 0, false, dir + M_PI / 2);
  //drawStillFrame(getAnim("select.png"), (x - world->viewer->x) * scale, (y - world->viewer->y) * scale, 0, false);
  if(innerWorld->targetPath != NULL) {
    for(int i = 0; i < innerWorld->targetPath->size(); i++) {
      Loc loc = (*innerWorld->targetPath)[i];
      //drawStillFrame(getAnim("select-green.png"), (loc.x - world->viewer->x) * scale, (loc.y - world->viewer->y) * scale, 0, false);
    }
    
  }
  //drawRect(screen, (x - world->viewer->x) * world->zoom, (y - world->viewer->y) * world->zoom, world->zoom, world->zoom, (Color) {255, 0, 0});
  //innerWorld->draw(screen);
}

// todo: fix bug that occurs when off world axes here (x axis seems to be problematic, not y)
void Bot::updateBlocksInVision() {
  for(int i = max(x - visionRange / 2, 0); i < min(x + visionRange / 2, world->width); i++) {
    for(int j = max(y - visionRange / 2, 0); j < min(y + visionRange / 2, world->height); j++) {
      innerWorld->map->grid[i][j]->type = world->grid[i][j]->type;
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