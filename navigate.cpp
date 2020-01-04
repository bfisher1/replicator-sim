#include "navigate.hpp"
#include <cmath>
#include <queue> 
#include <map>
#include <stdio.h>
#include <iostream>
#include <string>

// for diagonal distances
#define SQRT_2 1.4142135

class Node;

class Node {
  public:
    Node *prev;
    double dist;
    Loc loc;
    int pos;
    Node(Loc l) {
      loc = l;
    }
    string toString() {
      string str;
      char nodeBuffer[25];
      sprintf(nodeBuffer, "%x", this);
      return loc.toString() + "=" + string(nodeBuffer);
    }
};

struct CmpNodePtrs
{
  bool operator()(const Node* a, const Node* b) const
  {
    return a->dist > b->dist;
  }
};

class Edge {
  public:
    Node *dest;
    double cost;
    Edge(Node *node, double d) {
      dest = node;
      cost = d;
    }
};

// uses a graph implementation that has redundant edges (each edge is stored twice)
// however, this allows for an efficient lookup
class Graph {
  public:
    vector<Node *> nodes;
    map<Node *, vector<Edge *> *> *neighbors;
    void addEdge(Node *src, Node *dest, double cost) {
      Edge *destEdge = new Edge(dest, cost);
      neighbors->at(src)->push_back(destEdge);
      // Edge *srcEdge = new Edge(src, cost);
      // neighbors->at(dest)->push_back(srcEdge);
    }

    void addNode(Node *node) {
      //cout << "AAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
      printMagic();
      //cout << "CREATING NODE" << endl;
      vector<Edge *> *edges = new vector<Edge*>();
      (*neighbors)[node] = edges;
      nodes.push_back(node);
      //cout << "node added " << node->toString() << "edge list cap = " << edges->capacity() << endl;
      //cout << "BBBBBBBBBBBBBBBBBBB" << endl;
      printMagic();
    }

    vector<Edge *> *getNeighbors(const Node *node) {
      return neighbors->at((Node *) node);
    }

    Graph() {
      neighbors = new map<Node *, vector<Edge *> *>();
    }

  void printMagic() {
    for(int i = 0; i < nodes.size(); i++) {
      //cout << getNeighbors(nodes[i])->size() << " " << getNeighbors(nodes[i])->capacity() << endl;
    }
    //cout << "----------------------" << endl;
  }

  void magic() {
    for(int i = 0; i < nodes.size(); i++) {
      if(getNeighbors(nodes[i])->capacity() > 100) {
        int x = 1 / 0;
        //cout << "Now your failure is complete" << x << endl;
        throw "Failure";
        exit(1);
      }
    }
  }

  string toString() {
    string s = "";
    for(int i = 0; i < nodes.size(); i++) {
      s += nodes[i]->toString() + "{";
      vector<Edge *> *neighbors = getNeighbors(nodes[i]);
      for(int j = 0; j < neighbors->size(); j++) {
        s += (*neighbors)[j]->dest->toString() + " . ";
      }
      s += "}\n";
    }
    return s;    
  }

};

Graph *mapToGraph(InnerWorld *innerWorld, Loc target, Node *&start, Node *&goal) {
  Graph *graph = new Graph();
  Loc src = Loc(innerWorld->self->x, innerWorld->self->y);
  // get bounding box of src and target
  Loc topLeft;
  Loc bottomRight;
  //cout << "Bounding: " << src.toString() << "; " << target.toString() << endl;
  Loc::setBoundingBox(src, target, topLeft, bottomRight);
  // then convert all blocks that are crossable to graph
  
  int width = bottomRight.x - topLeft.x + 2;
  int height = bottomRight.y - topLeft.y + 2;
  int startX =  (int) topLeft.x;
  int startY =  (int) topLeft.y;
  // create a 2d array of the pointers to the nodes (leave uncrossable blocks as NULL)
  vector<vector<Node *> *> *crossableNodes = new vector<vector<Node *> *>();
  
  //cout << "ALG bot = " << innerWorld->botLoc().x << " " << innerWorld->botLoc().y << "; target = " << target.x << " " << target.y << endl;
  //cout << "Bounding box" << topLeft.toString() << "; " << bottomRight.toString() << endl;
  //cout << "width = " << width << "; height = " << height << endl;
  for(int j = 0; j < width; j++) {
    crossableNodes->push_back(new vector<Node *>());
    for(int i = 0; i < height; i++) {
      //cout << "00000000000" << endl;
      graph->printMagic();
      ////cout << "    i & j: " << i << " " << j << endl;
      if(innerWorld->map->isCrossable(innerWorld->map->grid[j + startX][i + startY]->type)) {
        Node *node = new Node(Loc(startX + j, startY + i));
        //cout << "    i & j: " << i << " " << j << "-> global -> " << node->loc.toString() << endl;
        //cout << "00000000000" << endl;
        graph->printMagic();
        // add node to 2d array
        (*crossableNodes)[j]->push_back(node);
        // add node to graph
        graph->addNode(node);
        if(Loc::sameLoc(target, node->loc)) {
          goal = (*(*crossableNodes)[j])[i];
          //cout << "GOAL " << goal << endl;
        }
        else if(Loc::sameLoc(innerWorld->botLoc(), node->loc)) {
          start = (*(*crossableNodes)[j])[i];
          //cout << "START " << start << endl;
        }
        //graph->printMagic();
      }
      else {
        (*crossableNodes)[j]->push_back(NULL);
      }
    }
  }

  graph->magic();

  //cout << "map size " << graph->neighbors->size() << endl;
  
  // add the nodes and the edges to the graph
  for(int j = 0; j < width; j++) {
    for(int i = 0; i < height; i++) {
      //graph->magic();
      if((*(*crossableNodes)[j])[i] != NULL) {
        if(i - 1 >= 0 && (*(*crossableNodes)[j])[i - 1] != NULL) {
          //graph->magic();
          graph->addEdge((*(*crossableNodes)[j])[i], (*(*crossableNodes)[j])[i - 1], 1);
          //cout << "edge added" << endl;
        }
        if(i + 1 < height && (*(*crossableNodes)[j])[i + 1] != NULL) {
          graph->addEdge((*(*crossableNodes)[j])[i], (*(*crossableNodes)[j])[i + 1], 1);
        }
        if(j - 1 >= 0 && (*(*crossableNodes)[j - 1])[i] != NULL) {
          graph->addEdge((*(*crossableNodes)[j])[i], (*(*crossableNodes)[j - 1])[i], 1);
        }
        if(j + 1 < width && (*(*crossableNodes)[j + 1])[i] != NULL) {
          graph->addEdge((*(*crossableNodes)[j])[i], (*(*crossableNodes)[j + 1])[i], 1);
        }

        //diagonal edges
        // TODO Check that other blocks normal to diagonal are crossable too, otherwise can go thru diagonal
        if(i - 1 >= 0 && j - 1 >= 0 && (*(*crossableNodes)[j - 1])[i - 1] != NULL) {
          //graph->magic();
          graph->addEdge((*(*crossableNodes)[j])[i], (*(*crossableNodes)[j - 1])[i - 1], SQRT_2);
          //cout << "edge added" << endl;
        }
        if(i + 1 < height && j + 1 < width && (*(*crossableNodes)[j + 1])[i + 1] != NULL) {
          graph->addEdge((*(*crossableNodes)[j])[i], (*(*crossableNodes)[j + 1])[i + 1], SQRT_2);
        }
        if(i + 1 < height && j - 1 >= 0 && (*(*crossableNodes)[j - 1])[i + 1] != NULL) {
          graph->addEdge((*(*crossableNodes)[j])[i], (*(*crossableNodes)[j - 1])[i + 1], SQRT_2);
        }
        if(i - 1 >= 0 && j + 1 < width && (*(*crossableNodes)[j + 1])[i - 1] != NULL) {
          graph->addEdge((*(*crossableNodes)[j])[i], (*(*crossableNodes)[j + 1])[i - 1], SQRT_2);
        }
      }
    }
  }

  return graph;
}


Node *popMinNode(vector<Node *> *queue) {
  if(queue->size() == 0) {
    return NULL;
  }
  int idx = 0;
  Node *min = (*queue)[0];
  for(int i = 0; i < queue->size(); i++) {
    if( (*queue)[i]->dist < min->dist) {
      min = (*queue)[i];
      idx = i;
    }
  }
  queue->erase(queue->begin() + idx);
  return min;
}

/*
  Based on algorithm implementation from:
  http://www.gitta.info/Accessibiliti/en/html/Dijkstra_learningObject1.html
*/
vector<Loc> *djikstrasAlgorithm(Graph *graph, Node* start, Node *target) {
  vector<Loc> *points = new vector<Loc>();
  //return points;
  vector<Node *> *queue = new vector<Node *>();
  for(int i = 0; i < graph->nodes.size(); i++) {
    graph->nodes[i]->prev = NULL;
    graph->nodes[i]->dist = INFINITY;
    queue->push_back(&(*graph->nodes[i]));
  }

  start->dist = 0;

  while(queue->size() > 0) {
    const Node *u = popMinNode(queue);

    cout << "U: " << ((Node *) u)->loc.toString() << " neighbors = " << graph->getNeighbors(u)->size() << endl;

    vector<Edge *> *neighbors = graph->getNeighbors(u);
    for(int i = 0; i < neighbors->size(); i++) {
      Edge *neighborEdge = (*neighbors)[i];
      double alt = u->dist + neighborEdge->cost;
      cout << "----ALT " << alt << " = "  << u->dist << " + " << neighborEdge->cost << "; " << neighborEdge->dest->toString() << endl;
      if(alt < neighborEdge->dest->dist) {
        neighborEdge->dest->dist = alt;
        //queue.updateDist(neighborEdge->dest, neighborEdge->dest->dist);
        cout << "ALT " << neighborEdge->dest->loc.toString() << " <- " << ((Node *)u)->loc.toString() << endl;
        neighborEdge->dest->prev = (Node *) u;
      }
    }

    if(u == target) {
      
      for(Node *n = (Node *) u; n != NULL; n = n->prev) {
        points->push_back(n->loc);
      }
      cout << "Points:" <<endl;
      for(int i = 0; i < points->size(); i++) {
        cout << (*points)[i].toString() << " ";
      }
      cout << endl;
      cout << "TAARGET: " << points->size() << " " << (u == NULL) << endl;
      return points;
    }
  }
  cout << "GRAPH: " << endl;
  cout << graph->toString() << endl;
  cout << endl << "Start " << start->toString() << endl;
  cout << "Target " << target->toString() << endl;
  cout << "NODES" << endl;
  for(int i = 0; i < graph->nodes.size(); i++) {
    cout << graph->nodes[i]->toString() << ", " << graph->nodes[i]->dist << " " << (graph->nodes[i]->prev != NULL);
    if(Loc::sameLoc(graph->nodes[i]->loc, target->loc)) {
      cout << "  <------- Target";
    }
    else if(Loc::sameLoc(graph->nodes[i]->loc, start->loc)) {
      cout << "  <------- Start";
    }
    cout << endl;
  }

  // cout << "fail" << (start == NULL) << " " << (target == NULL) << endl;
  // cout << "     FAiled to run thru all points for " << start->loc.toString() << " to " << target->loc.toString() << endl;
  // for(int i = 0; i < graph->nodes.size(); i++) {
  //   cout << "     " << graph->nodes[i]->loc.toString() << endl;
  // }

  //exit(0);
  return points;
}

vector<Loc> *createNaiveDjikstraRoute(InnerWorld *innerWorld, Loc target) {
  
  vector<Loc> *points = new vector<Loc>();
  if(!innerWorld->isCrossable(innerWorld->botLoc()) || !innerWorld->isCrossable(target)) {
    int botx = innerWorld->botLoc().x;
    int boty = innerWorld->botLoc().y;
    cout << "Support not available" << innerWorld->map->grid[botx][boty]->name << " at " << innerWorld->botLoc().toString() << endl;
    return points;
  }
  Node* start;
  Node* goal;
  Graph *graph = mapToGraph(innerWorld, target, start, goal);
  return djikstrasAlgorithm(graph, start, goal);
}