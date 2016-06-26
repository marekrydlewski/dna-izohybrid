#ifndef graph_hpp
#define graph_hpp

#include <stdio.h>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include <stack>
#include <vector>
#include <climits>

#include "node.hpp"
#include "dna_oligonucleotides.hpp"

class Graph {
private:
	std::map<std::string, Node*> nodes;

	std::stack<Node*> stack;
	std::map<Node*, int> visited;
	std::stack<Node*> finalStack;
	int sum = 0, maxSum = INT_MAX, length;

	bool found = false;

	Node* getOrCreate(std::string value, int count);
	bool endsWith(std::string const &value, std::string const &ending);
	bool startsWith(std::string const &value, std::string const &ending);
	bool allVisited();
	void DFS(Node * node);
public:
	std::string toString(std::vector<std::string> path);
	void getOrCreateEdge(Node * v_i, Node * v_j, int overlap);
	void dealWithOligo(Node * v_i, Node * v_j);
	int getOverlap(std::string s1, std::string s2);
	//Graph(std::string data, int k);
	Graph::Graph(std::map<std::string, OligoNumbers> oligoMap, std::string firstOligo, int length);
	std::vector<std::string> getPath();
	std::vector<std::string> getFinalPath();
	bool containsAllOligos();
	//std::vector<Node*> eulerianPath();
};

#endif /* graph_hpp */
