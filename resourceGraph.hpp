#ifndef RESOURCE_GRAPH_H
#define RESOURCE_GRAPH_H
#include <string>
#include <vector>
#include <map>
#include "resourceGraph.hpp"
#define DEFAULT_QUANTITY_NEEDED 1
#define DEFAULT_QUANTITY_PRODUCED 1

using namespace std;

class ResourceInput {
  public:
    string name;
    double amountRequired;
    ResourceInput(string inputString);
};

class ResourceOutput {
  public:
    string name;
    double amountCreated;
    ResourceOutput(string outputString);
};

class Action {
  public:
    vector<string> preconds;
    vector<string> effects;
    string name;
    double duration;
    Action(string name);
};

// the result of a list of inputs to create an output
class ResourceProduct {
  public:
    ResourceOutput *output;
    Action *action;
    vector<ResourceInput*> *inputs;
    ResourceProduct(ResourceOutput *output, Action *action, vector<ResourceInput *> *inputs);
};

class ResourceGraph {
  public:
    vector<string> raw;
    vector<string> molds;
    vector<string> producers;

    // name to action
    map<string, Action *> actions;
    
    // name of output to resourceProduct
    map<string, ResourceProduct *> resources;
    
    ResourceGraph(string fileName);
    void print();
};
#endif /* RESOURCE_GRAPH_H */