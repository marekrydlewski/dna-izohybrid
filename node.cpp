#include "node.hpp"

Node::Node(std::string value, int count) {
	this->_value = value;
	this->_count = count;
}

/*
bool Node::isBalanced() {
return _in == _out;
}

bool Node::isSemiBalanced() {
return abs(_in - _out) == 1;
}
*/
