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
    if (!nodes.count(value))
        nodes[value] = new Node(value);
    return nodes[value];
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
    for (auto o_i : oligoMap) {
        for (auto o_j : oligoMap) {
            Node *v_i = getOrCreate(o_i.first), *v_j = getOrCreate(o_j.first);
            if (v_i == v_j)
                break;
            if (startsWith(v_i->value(), v_j->value())) {
                if (!v_j->isLeaveLocked() && !v_i->isEnterLocked()) {
                    v_j->setOut(v_j->out() + 1);
                    v_i->setIn(v_i->in() + 1);
                    if (G.count(v_j) == 0)
                        G[v_j] = new std::vector<Node*>();
                    G[v_j]->push_back(v_i);
                    v_i->lockLeave();
                    v_j->lockEnter();
                }
            }
            else if (endsWith(v_i->value(), v_j->value())) {
                if (!v_i->isLeaveLocked() && !v_j->isEnterLocked()) {
                    v_i->setOut(v_i->out() + 1);
                    v_j->setIn(v_j->in() + 1);
                    if (G.count(v_i) == 0)
                        G[v_i] = new std::vector<Node*>();
                    G[v_i]->push_back(v_j);
                    v_j->lockLeave();
                    v_i->lockEnter();
                }
            }
//            else if ((v_i->value().length() == v_j->value().length()) &&
//                  endsWith(v_i->value(), v_j->value().erase(v_j->value().length() - 1))) {
                //TODO: add arc from v_i to v_j
                //on condition the overlap does not produce negative errors (?)
//            }
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
