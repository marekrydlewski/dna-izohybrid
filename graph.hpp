#ifndef graph_hpp
#define graph_hpp

#include <stdio.h>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include <stack>
#include <vector>

#include "node.hpp"
#include "dna_oligonucleotides.hpp"

class Graph {
private:
	std::map<std::string, Node*> nodes;

	std::stack<Node*> stack;
	std::map<Node*, int> visited;

	Node* getOrCreate(std::string value, int count);
	bool endsWith(std::string const &value, std::string const &ending);
	bool startsWith(std::string const &value, std::string const &ending);
	void dealWithOligo(Node*, Node*, bool);
	bool allVisited();
	void DFS(Node * node);
public:
	//Graph(std::string data, int k);
	Graph(DnaOligonucleotides dna, bool);
	std::vector<std::string> getPath();
	//std::vector<Node*> eulerianPath();
};

#endif /* graph_hpp */
