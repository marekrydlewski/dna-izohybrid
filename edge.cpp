#include "edge.h"

Edge::Edge(Node *source, Node *dest, int weight)
{
	_source = source;
	_dest = dest;
	_weight = weight;
}