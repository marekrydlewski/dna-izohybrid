#ifndef node_hpp
#define node_hpp

#include <string>
#include <cstdlib>
#include <vector>

#include "edge.h"

class Edge;

class Node {
private:
	//int _in = 0, _out = 0;
	//bool _isEnterLocked = false, _isLeaveLocked = false;
	std::string _value;
	int _count;
public:
	Node(std::string value, int count);
	//bool isBalanced();
	//bool isSemiBalanced();
	//int in() { return _in; }
	//int out() { return _out; }
	//void setIn(int in) { _in = in; }
	//void setOut(int out) { _out = out; }
	//bool isEnterLocked() { return _isEnterLocked; }
	//bool isLeaveLocked() { return _isLeaveLocked; }
	//void lockEnter() { _isEnterLocked = true; }
	//void lockLeave() { _isLeaveLocked = true; }
	std::string value() { return _value; }
	std::vector<Edge*> edges;
	int count() { return _count; }
};

#endif /* node_hpp */
