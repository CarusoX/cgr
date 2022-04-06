#ifndef NODE_H
#define NODE_H

class Node {

  bool canGoTo(Node* neighbour, double time);

  double edgeCost(Node* neighbour, double time);
};

#endif