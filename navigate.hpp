#ifndef NAVIGATE_H
#define NAVIGATE_H
#include "loc.hpp"
#include "innerWorld.hpp"
#include <vector>

using namespace std;

class InnerWorld;

vector<Loc> *createNaiveDjikstraRoute(InnerWorld *innerWorld, Loc target);

#endif