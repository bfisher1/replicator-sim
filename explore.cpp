#include "explore.hpp"
#include "util.hpp"

#define RANGE_VARIANCE 5

ExploreNode::ExploreNode(Loc o) {
  origin = o;
  for(int i = 0; i < DIRECTION_NUM; i++) {
    exploredDir[i] = false;
  }
}

Explore::Explore(Direction initDir) {
  dir = initDir;
  history = vector<ExploreNode *>();
}


//TODO, add ranom noise circle to the target loc at end
Loc Explore::getNextTarget(Loc loc, World *world) {
  // if(history.size() == 0) {
  //   history.push_back(new ExploreNode(loc));
  // }

  // dest if we continue on this direction
  Loc dirDest = addVectorToLoc(loc, nodeDist, dir);
  if(world->grid[(int) dirDest.x][(int) dirDest.y]->type == BlockType::unknown) {
    // add a splash of randomness to the target
    cout << endl;
    cout << dirDest.toString() << endl;
    cout << endl;
    
    //dirDest = dirDest.plus(Loc(0, 0));//dirDest.plus(Loc(randRange(-RANGE_VARIANCE, RANGE_VARIANCE), randRange(-RANGE_VARIANCE, RANGE_VARIANCE)));
    // find the closest unknown block in this direction
    for(int len = nodeDist; len >= 0; len--) {
      Loc dirDestCloser = addVectorToLoc(loc, len, dir);
      if(world->grid[(int) dirDestCloser.x][(int) dirDestCloser.y]->type == BlockType::unknown) {
        dirDest = dirDestCloser;
      }
      else {
        break;
      }
    }
    // get block in x that's furthest from solid blocks

    dirDest.x += randRange(-RANGE_VARIANCE, RANGE_VARIANCE);
    int minX = max(0, dirDest.x - RANGE_VARIANCE);
    int maxX = min(world->width, dirDest.x + RANGE_VARIANCE);
    double scores[maxX - minX];
    // assign scores to 0
    for(int i = minX; i < maxX; i++) {
      scores[i - minX] = 0;
    }
    // iterate and furthest block
    for(int i = minX; i < maxX; i++) {
      for(int j = minX; j < maxX; j++) {
        if(world->grid[j][(int) dirDest.y]->crossable) {
          // add by 1 / distance (closer ones will add more to it)
          scores[i - minX] += 1.0 / fabs(i - j);
        }
      }
    }
    int idx = 0;
    //dirDest.x = scores[0];
    for(int i = minX; i < maxX; i++) {
      if(scores[i - minX] < scores[idx]) {
        dirDest.x = scores[i - minX];
        idx = i - minX;
      }
    }
    //dirDest.y += randRange(-RANGE_VARIANCE, RANGE_VARIANCE);
    return dirDest;
  }
  cout << "Not implemented yet" << endl;
  exit(0);
  return Loc(-1, -1);

  // add targets for other 3 directions to a vector
  // todo randomize this order
  vector<Loc> otherDirTargets = vector<Loc>();
  // these should be other directions
  otherDirTargets.push_back(addVectorToLoc(loc, nodeDist, dir));
  otherDirTargets.push_back(addVectorToLoc(loc, nodeDist, dir));
  otherDirTargets.push_back(addVectorToLoc(loc, nodeDist, dir));
  
  while(otherDirTargets.size() > 0) {
    Loc targetLoc = otherDirTargets.back();
    if(world->grid[(int) targetLoc.x][(int) targetLoc.y]->type == BlockType::unknown) {
      // TODO mark off this one as exploredDir
      return targetLoc;
    }
    otherDirTargets.pop_back();
  }

  cout << "HAVE TO TURN BACK" << endl;
  exit(0);
  return Loc(-1, -1);

}
