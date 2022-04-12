#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include "node.hpp"

template <class T, class S>
concept CheckType = std::is_base_of<S, T>::value;

template <class T>
requires CheckType<T, Node>
class GraphNode {
public:
  T* node;
  bool supressed;
  GraphNode(T* _node) : node(_node), supressed(0) {}
};

template <class T>
using GraphNodeT = GraphNode<T>*;

#endif