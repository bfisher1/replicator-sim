#include "path.hpp"
#include <unistd.h>

PathIterator::PathIterator(vector<Loc> *p, Bot *b) {
  path = p;
  bot = b;
  index = 0;
  for(int i = 0; i < path->size(); i++) {
    Loc loc = (*path)[i];
    if(Loc::distWithin(loc, Loc(bot->x, bot->y), .1)) {
      index = i;
    }
  }
}

bool PathIterator::inc(double velocity) {
  Loc loc = (*path)[index];
  if(Loc::distWithin(loc, Loc(bot->x, bot->y), .1)) {
    index++;
    cout << "next block " << loc.toString() << " at idx: " << index << " bot at: " << Loc(bot->x, bot->y).toString();
    cout << Loc::dist(loc, Loc(bot->x, bot->y)) << endl;
    loc = (*path)[index];
    // sleep(1);
  }
  else {
    cout << "----------  " << Loc::dist(loc, Loc(bot->x, bot->y)) << endl;
  }
  if(index >= path->size()) {
    // means we have finished the path
    return true;
  }
  //cout << loc.toString() <<  " vs " << bot->x << " " << bot->y << ": " << index << endl;
  bot->face(loc);
  //bot->x = loc.x;
  //bot->y = loc.y;
  bot->vel = velocity;
  //sleep(1);
  return false;
}