#ifndef graph_hpp
#define graph_hpp

#include <stdio.h>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>

#include "node.hpp"

class Graph {
private:
    int nsemi = 0, nbal = 0, nneither = 0;
    Node *head, *tail;
    std::vector<std::string *> chop(std::string data, int k); // to delete

    std::vector<Node*> tour;
    std::map<std::string, Node*> nodes;
    std::map<Node*,std::vector<Node*>*> G, g;

    bool hasEulerianPath() { return nneither == 0 && nsemi == 2; }
    bool hasEulerianCycle() { return nneither == 0 && nsemi == 0; }
    bool isEulerian() { return hasEulerianCycle() || hasEulerianPath(); }
    void visit(Node *node);

    Node* getOrCreate(std::string value);
    bool endsWith(std::string const &value, std::string const &ending);
    bool startsWith(std::string const &value, std::string const &ending);
public:
    Graph(std::string data, int k);
    Graph(std::map<std::string, int> oligoMap);
    std::vector<Node*> eulerianPath();
};

#endif /* graph_hpp */
