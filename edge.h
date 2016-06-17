#ifndef edge_hpp
#define edge_hpp

#include "node.hpp"

class Node;
class Edge
{
	Node *_source, *_dest;
	int _weight = 0;
public:
	Edge(Node *source, Node *dest, int weight);
	Node* source() { return _source; }
	Node* dest() { return _dest; }
	int weight() { return _weight; }
};

#endif /* edge_hpp */