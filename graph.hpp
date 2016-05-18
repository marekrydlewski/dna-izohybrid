#ifndef graph_hpp
#define graph_hpp

#include <stdio.h>
#include <vector>
#include <tuple>
#include <map>

#include "node.hpp"

class Graph {
private:
    int nsemi = 0, nbal = 0, nneither = 0;
    Node *head, *tail;
    std::vector<std::string *> chop(std::string data, int k);
    bool hasEulerianPath() { return nneither == 0 && nsemi == 2; }
    bool hasEulerianCycle() { return nneither == 0 && nsemi == 0; }
    bool isEulerian() { return hasEulerianCycle() || hasEulerianPath(); }
    void visit(Node *node);
    std::vector<Node*> tour;
    std::map<std::string, Node*> nodes;
    std::map<Node*,std::vector<Node*>*> G, g;
public:
    Graph(std::string data, int k);
    std::vector<Node*> eulerianPath();
};

#endif /* graph_hpp */
