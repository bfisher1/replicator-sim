#ifndef EXPLORE_H
#define EXPLORE_H
#include "direction.hpp"
#include "world.hpp"

class World;

class ExploreNode {
  public:
    Loc origin;
    bool exploredDir[DIRECTION_NUM];
    ExploreNode(Loc origin);
};

class Explore {
  public:
    Direction dir;
    vector<ExploreNode *> history;
    Explore(Direction initDir);
    // distance between explore nodes and changing dir
    int nodeDist = 10;
    Loc getNextTarget(Loc loc, World *world);
};

#endif /* EXPLORE_H */