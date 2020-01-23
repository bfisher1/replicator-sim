#include "bot.hpp"
#include "util.hpp"
#include <iostream>
#include "anim.hpp"
#include "direction.hpp"
#include <unistd.h>

#define TURN_SENSITIVITY 1.62

void wanderUpdateMove(Bot *bot) {
  //bot->vel = .04;
  //bot->dir += TURN_SENSITIVITY * (randNorm() - .5);
  bot->vel = .0514;
  //bot->dir = -(3.14 / 2 - .6);
}

// used for comparing locs, done on a distance basis
static bool operator<(const Loc& l1, const Loc& l2) {
  return l1.x * l1.x + l1.y * l1.y < l2.x * l2.x + l2.y * l2.y;
}


void *execBot(void *arg) {
  Bot *bot = (Bot *) arg;
  bot->exec();
  return NULL;
}

void Bot::exec() {
  while(true) {
    cout << "-            Updatiing" << dir << endl;
    dir += .00000001;
    update();
    
  }
}

Mob::Mob(World *world) {
  this->world = world;
  started = false;
}

void Mob::start() {
  if ( pthread_create( &thread, NULL, execBot, this) != 0) {
    printf("Couldn't create thread for mob\n");
    exit(0);
  }
  started = true;
}

// this runs at each tick, wanderUpdateMove only happens after a fraction of a second
void wanderUpdateMoveConstant(Bot *bot) {
  if(bot->pressed) {
    bot->dir += M_PI;
    cout << "turn" << endl;
  }
  bot->innerWorld->deselectAll();
  bool notCrossableBlock = false;
  for(int offset = -20; offset < 21; offset += 1) {
    double offsetRadians = offset * M_PI / 180.0;
    for(double radius = 1; radius < 4; radius++) {
      
      double doublex = (radius * cos(bot->dir + offsetRadians) + bot->x);
      double doubley = (radius * sin(bot->dir + offsetRadians) + bot->y);
      int x = doublex;
      int y = doubley;

      cout << "loc " << Loc(doublex, doubley).toString() << " " << bot->innerWorld->plots->size() << endl;
      bot->innerWorld->plots->push_back(Loc(doublex, doubley));
      Block *block = bot->world->grid[x][y];
      bot->innerWorld->select(Loc(x, y));
      if(!block->crossable) {
        bot->innerWorld->highLightPlots->insert(bot->innerWorld->plots->size() - 1);
        // bot->innerWorld->select(Loc(x, y));
        notCrossableBlock = true;
        //return;
      }
    }
  }
  if(notCrossableBlock) {
    bot->vel = 0;
    bot->dir += M_PI / 100; //M_PI / 10;
  }
}

static bool flag = false;

bool Bot::traverseTargetPath() {
  // cout << "Target path:" << endl;
  // for(int i = 0; i < innerWorld->targetPath->size(); i++) {
  //   cout << (*innerWorld->targetPath)[i].toString() << " ";
  // }
  cout << "TRAVERSING +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl; 
  for(int i = 0; i < innerWorld->targetPath->size(); i++) {
    Loc loc = (*innerWorld->targetPath)[i];
    cout << "   " << loc.toString() << "; vs " << x << " " << y << endl;
    if(Loc::sameLoc(Loc(x, y), loc)) {
      vel = .05;
      // don't face if finished
      Loc nextLoc = (*innerWorld->targetPath)[i + 1];
      face(nextLoc);
      // x = nextLoc.x;
      // y = nextLoc.y;
      // cout << "I " <<  i << endl;
      // sleep(1);
      cout << "MOVING TO " << i << endl;
      return false;
    }
  }
  vel = .05;
  face((*innerWorld->targetPath)[0]);
  cout << "MOVING TO INIT" << endl;
  return true;
    
}




void exploreUpdateMove(Bot *bot) {

  //cout << "pressed " << bot->pressed << endl;
  if(bot->pressed) {
    // bot->pressed = false;
    flag = false;
    bot->vel = 0;
    wanderUpdateMove(bot);
  }
  
  if(flag) {
    if(bot->innerWorld->targetPathIterator->inc(.05)) {
      // have to create a new path now that the path has been incremented
      flag = false;
    }
    //bot->traverseTargetPath();
    return;
  }
  flag = true;
  // have general initial direction
  // keep exploring in that direction (select block in that direction)
  // then try exploring another direction (orthoganol). retrace if all directions seem explored
  //    need to keep a stack of direction history
  bot->innerWorld->toggleTargetPathSelect(false);
  bot->innerWorld->target = bot->innerWorld->explore->getNextTarget(Loc(bot->x, bot->y), bot->innerWorld->map);
  //bot->innerWorld->select(bot->innerWorld->target);
  bot->innerWorld->targetPath = bot->createRoute(bot->innerWorld->target);
  cout << "SIZE" << bot->innerWorld->targetPath->size() << endl;
  bot->innerWorld->toggleTargetPathSelect(true);
  bot->innerWorld->targetPathIterator = new PathIterator(bot->innerWorld->targetPath, bot);
}

void Bot::face(Loc loc) {
  dir = atan2(loc.y - y, loc.x - x);
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
    bot->pressed = false;
    cout << "Pressed stop " << endl;
  }
  else {
    // bot->dir += M_PI;
    bot->pressed = true;
    cout << "Pressed" << endl;
    // bot->vel = 0;
  }
  // cout << "top left " << bot->world->isCrossable(Loc(left,top)) << bot->world->grid[left][top]->name << endl;
  // cout << "top right " << bot->world->isCrossable(Loc(right,top)) << bot->world->grid[right][top]->name << endl;
  // cout << "bottom left " << bot->world->isCrossable(Loc(left,bottom)) << bot->world->grid[left][bottom]->name << endl;
  // cout << "bottom right " << bot->world->isCrossable(Loc(right,bottom)) << bot->world->grid[right][bottom]->name << endl;
  
  // cout << "----" << endl;
  
  //bot->innerWorld->focus = new Loc(right, bottom);  
}

void Bot::update() {

  updateBlocksInVision();
  return;
  innerWorld->plots = new vector<Loc>();
  innerWorld->highLightPlots = new set<int>();
  time_t now;
  time(&now);
  wanderUpdateMoveConstant(this);
  if(now > innerWorld->lastUpdate) {
    innerWorld->lastUpdate = now;
    wanderUpdateMove(this);
  }
  
  //exploreUpdateMove(this);
  move(this);
  // innerWorld->target = Loc(10, 10);
  // innerWorld->map->select(innerWorld->target, true);
  return;
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
      // TODO: THIS SHOULD BE COPY BY VALUE, not just reference
      innerWorld->map->grid[i][j] = world->grid[i][j];
    }
  }
}

Bot::Bot(int sx, int sy, World *world) : Mob(world) {
  x = sx;
  y = sy;
  visionRange = BOT_VISION_RANGE;
  innerWorld = new InnerWorld(world, this);
  pressed = false;
}

vector<Loc> *Bot::createRoute(Loc target) {
  vector<Loc> *locs = createNaiveDjikstraRoute(innerWorld, target);
  return locs;
}

/*
wander()
search() = goto stones, then wander and mine
*/