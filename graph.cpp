#include "graph.hpp"

#include <iostream>

std::vector<std::string *> Graph::chop(std::string data, int k) {
    std::vector<std::string *> partial;
    for (int i = 0; i < data.length() - k + 1; i++) {
        std::string bk = data.substr(i, k);
        partial.push_back(new std::string[3] { bk,
            bk.substr(0, k - 1),
            bk.substr(bk.size() - k + 1)});
    }
    return partial;
}

/*
 * deprecated - we need to create algorithm for izotermic sequencing :(
 */

Graph::Graph(std::string data, int k) {
    auto partial = this->chop(data, k);
    for (auto it = partial.begin() ; it != partial.end(); ++it) {
        Node *nodeL, *nodeR;
        if (nodes.count((*it)[1]))
            nodeL = nodes[(*it)[1]];
        else {
            nodeL = new Node((*it)[1]);
            nodes[(*it)[1]] = nodeL;
        }
        if (nodes.count((*it)[2]))
            nodeR = nodes[(*it)[2]];
        else {
            nodeR = new Node((*it)[2]);
            nodes[(*it)[2]] = nodeR;
        }
        nodeL->setOut(nodeL->out() + 1);
        nodeR->setIn(nodeR->in() + 1);
        if (G.count(nodeL) == 0)
            G[nodeL] = new std::vector<Node*>();
        G[nodeL]->push_back(nodeR);
    }
    for (auto it = nodes.begin() ; it != nodes.end(); ++it) {
        Node* node = it->second;
        if (node->isBalanced())
            nbal++;
        else if (node->isSemiBalanced()) {
            node->in() == node->out() + 1 ? tail = node : head = node;
            nsemi += 1;
        }
        else
            nneither += 1;
    }
}

Node* Graph::getOrCreate(std::string value) {
    Node *node;
    if (nodes.count(value))
        return nodes[value];
    else {
        node = new Node(value);
        nodes[value] = node;
        return node;
    }
}

bool Graph::endsWith(std::string const &value, std::string const &ending)
{
    return (ending.size() <= value.size())
        && std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool Graph::startsWith(std::string const &value, std::string const &starting) {
    return (starting.length() <= value.length())
        && std::equal(starting.begin(), starting.end(), value.begin());
}

Graph::Graph(std::map<std::string, int> oligoMap) {
    for (auto i : oligoMap) {
        for (auto j : oligoMap) {
            Node *nodeL = getOrCreate(i.first), *nodeR = getOrCreate(j.first);
            if (nodeL == nodeR)
                break;
            // step 2 here
            if (startsWith(nodeL->value(), nodeR->value())) {
                //TODO: add arc from v_j to v_i
                printf("%s starts with %s\n", nodeL->value().c_str(), nodeR->value().c_str());
            }
            else if (endsWith(nodeL->value(), nodeR->value())) {
                //TODO: add arc from v_i to v_j
                printf("%s ends with %s\n", nodeL->value().c_str(), nodeR->value().c_str());
            }
            else if ((nodeL->value().length() == nodeR->value().length()) &&
                  endsWith(nodeL->value(), nodeR->value().erase(nodeR->value().length() - 1))) {
                //TODO: add arc from v_i to v_j
                //on condition the overlap does not produce negative errors (?)
                printf("%s and %s are overlaping\n", nodeL->value().c_str(), nodeR->value().c_str());
            }
        }
    }
}

void Graph::visit(Node *node) {
    while (g[node] != NULL && g[node]->size() > 0) {
        Node *dst = g[node]->back();
        g[node]->pop_back();
        visit(dst);
    }
    tour.push_back(node);
}

std::vector<Node*> Graph::eulerianPath() {
    tour.clear();
    g = G;
    if (hasEulerianPath()) {
        if (G.count(tail) == 0)
            G[tail] = new std::vector<Node *>();
        G[tail]->push_back(head);
    }
    visit(head);
    std::reverse(tour.begin(), tour.end());

    if (hasEulerianPath()) {
        long index = std::distance(tour.begin(), find(tour.begin(), tour.end(), head));
        auto first = tour.begin() - index;
        auto last = tour.end() - index;
        tour = std::vector<Node *>(first, last);
    }
    return tour;
}
