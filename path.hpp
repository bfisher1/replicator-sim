#ifndef PATH_H
#define PATH_H

#include "loc.hpp"
#include <vector>
#include "bot.hpp"

class Bot;

class PathIterator {
  public:
    int index;
    PathIterator(vector<Loc> *path, Bot *bot);
    vector<Loc> *path;
    Bot *bot;
    bool inc(double velocity);
};

#endif /*PATH_H*/