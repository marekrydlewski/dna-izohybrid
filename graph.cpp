#include "graph.hpp"

#include <iostream>

Node* Graph::getOrCreate(std::string value, int count) {
	if (!nodes.count(value))
		nodes[value] = new Node(value, count);
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

void Graph::dealWithOligo(Node *v_i, Node *v_j, bool negative) {
	int count = negative ? v_i->value().length() : 2;
	for (int i = 1; i < count; i++) {
		if (v_j->value().length() + i < v_i->value().length())
			continue;
		std::string str1 = v_i->value().substr(i);
		std::string str2 = v_j->value().substr(0, str1.length());
		if (std::equal(str1.begin(), str1.end(), str2.begin())) {
			v_i->edges.push_back(new Edge(v_i, v_j, i));
		}
	}
}

Graph::Graph(DnaOligonucleotides dna, bool negative) {
	Node *firstNode = NULL;
	auto oligoMap = dna.getOligoMap();
	for (auto o_i : oligoMap) {
		for (auto o_j : oligoMap) {
			Node *v_i = getOrCreate(o_i.first, o_i.second), *v_j = getOrCreate(o_j.first, o_j.second);
			if (v_i->value() == dna.getFirstOligo())
				firstNode = v_i;
			if (v_i == v_j)
				continue;
			dealWithOligo(v_i, v_j, negative);
		}
	}
	for (auto n : nodes) {
		visited[n.second] = n.second->count();
	}
	if (firstNode != NULL)
		DFS(firstNode);
}

bool Graph::allVisited() {
	for (auto v : visited) {
		if (v.second)
			return false;
	}
	return true;
}

std::vector<std::string> Graph::getPath() {
	std::vector<std::string> data;
	std::stack<Node*> stack_copy = stack;
	std::string str;
	while (!stack_copy.empty())
	{
		data.push_back(stack_copy.top()->value());
		stack_copy.pop();
	}
	std::reverse(data.begin(), data.end());
	return data;
}

void Graph::DFS(Node *node) {
	stack.push(node);
	if (stack.size() < nodes.size()) {
		visited[node]--;
		for (auto e : node->edges) {
			if (visited[e->dest()] > 0)
				DFS(e->dest());
			else {
			}
		}
		visited[node]++;
	}
	else if (allVisited())
	{
		getPath();
		return;
	}
	stack.pop();
}