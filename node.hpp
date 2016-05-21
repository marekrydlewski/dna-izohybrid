#ifndef node_hpp
#define node_hpp

#include <string>
#include <cstdlib>

class Node {
private:
    int _in = 0, _out = 0;
    bool _isEnterLocked = false, _isLeaveLocked = false;
    std::string _value;
public:
    Node(std::string value);
    bool isBalanced();
    bool isSemiBalanced();
    int in() { return _in; }
    int out() { return _out; }
    void setIn(int in) { _in = in; }
    void setOut(int out) { _out = out; }
    bool isEnterLocked() { return _isEnterLocked; }
    bool isLeaveLocked() { return _isLeaveLocked; }
    void lockEnter() { _isEnterLocked = true; }
    void lockLeave() { _isLeaveLocked = true; }
    std::string value() { return _value; }
};

#endif /* node_hpp */
