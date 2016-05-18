#include "node.hpp"

Node::Node(std::string value) {
    this->_value = value;
}

bool Node::isBalanced() {
    return _in == _out;
}

bool Node::isSemiBalanced() {
    return abs(_in - _out) == 1;
}
