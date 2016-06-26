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


void Graph::dealWithOligo(Node *v_i, Node *v_j) {
	int count = v_i->value().length();
	for (int i = 1; i < count; i++) {
		if (v_j->value().length() + i < v_i->value().length())
			continue;
		std::string str1 = v_i->value().substr(i);
		std::string str2 = v_j->value().substr(0, str1.length());
		if (std::equal(str1.begin(), str1.end(), str2.begin())) {
			v_i->edges.push_back(new Edge(v_i, v_j, 1));
		}
	}
}

int Graph::getOverlap(std::string s1, std::string s2) {
	int max = 0; // max common substring (end s1 ___ begin s2) - overlap 
	int size = s1.size() > s2.size() ? s2.size() : s1.size();
	int j = 1;
	for (auto i = size - 1; i >= 0; --i) {
		bool isValid = true;
		for (auto jj = 0; jj < j; ++jj) {
			if (s1[s1.size() - 1 - j + 1 + jj] != s2[jj]) {
				isValid = false;
				break;
			}
		}
		if (isValid) {
			max = j;
		}
		++j;
	}
	return max;
}

std::string Graph::toString(std::vector<std::string> path)
{
	if (path.size() > 0) {
		std::string solution = path[0]; //first oligo
		for (auto i = 1; i < path.size(); ++i)
		{
			auto s1 = path[i - 1];
			auto s2 = path[i];
			int overlap = getOverlap(s1, s2);
			//auto beka = 
			//if (!endsWith(s1, s2))
			solution += s2.substr(overlap);
		}
		return solution;
	}
	else
		return std::string();
}

void Graph::getOrCreateEdge(Node *v_i, Node *v_j, int overlap) {
	bool found = false;
	for (auto e : v_i->edges) {
		if ((e->source() == v_i) && (e->dest() == v_j) && (e->weight() == overlap))
			found = true;
	}
	if (!found)
		v_i->edges.push_back(new Edge(v_i, v_j, overlap));

}

Graph::Graph(std::map<std::string, OligoNumbers> oligoMap, std::string firstOligo, int length) {
	 // {AAA,AAAT,AAT,ATG,GT,GTA,GTT,TAA,TAAA,TG,TGT} => A A A T G T A A A
	this->length = length;
	Node *firstNode = NULL;
	//std::vector<std::string> oligoMap2 { "AAA", "AAAT", "AAT","ATG","GT","GTA","TAA","TAAA","TG", "TGT" };
	for (auto o_i : oligoMap) {
		for (auto o_j : oligoMap) {
			Node *v_i = getOrCreate(o_i.first, (int)(o_i.second)), *v_j = getOrCreate(o_j.first, (int)(o_j.second));
			//Node *v_i = getOrCreate(o_i, 1), *v_j = getOrCreate(o_j, 1);
			if (v_i->value() == firstOligo)
			//if (v_i->value() == "AAAT")
				firstNode = v_i;
			if (v_i == v_j)
				continue;
			//dealWithOligo(v_i, v_j);
			int overlap = getOverlap(v_i->value(), v_j->value());
			if (overlap) {
				getOrCreateEdge(v_i, v_j, overlap);
			}
			//	v_i->edges.push_back();
		}
	}
	//printf("%d\n", getOverlap("AAA", "TAAA"));
	printf("%d\n", nodes.size());
	for (auto n : nodes) {
		visited[n.second] = n.second->count();
	}
	if (firstNode != NULL)
		DFS(firstNode);
}

bool Graph::allVisited() {
	for (auto v : visited) {
		if (v.second > 0)
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
	/*
	for (auto i = 1; i < data.size(); ++i)
	{
		auto s1 = oligoSpectrum[individual.oligos[i - 1]];
		auto s2 = oligoSpectrum[individual.oligos[i]];
		int overlap = getOverlap(s1, s2);
		solution += s1.substr(overlap);
	}
	*/
	return data;
}

std::vector<std::string> Graph::getFinalPath() {
	std::vector<std::string> data;
	std::stack<Node*> stack_copy = finalStack;
	std::string str;
	while (!stack_copy.empty())
	{
		data.push_back(stack_copy.top()->value());
		stack_copy.pop();
	}
	std::reverse(data.begin(), data.end());
	return data;
}

bool Graph::containsAllOligos() {
	std::string finalString = toString(getPath());
	for (auto n : nodes) {
		if (finalString.find(n.first) == std::string::npos) {
			return false;
		}
	}
	return true;
}
/*
int Graph::DFS(Node *node) {
	stack.push(node);
	visited[node]--;
	for (auto e : node->edges) {
		if (visited[e->dest()] > 0)
			return DFS(e->dest());
		else {
			return 1;
		}
	}
	visited[node]++;
	stack.pop();
	return 0;
}
*/

/*
void TSP(int v)
{
  int u;

  Sh[shptr++] = v;                // zapamiêtujemy na stosie bie¿¹cy wierzcho³ek

  if(shptr < n)                   // jeœli brak œcie¿ki Hamiltona, to jej szukamy
  {
    visited[v] = true;            // Oznaczamy bie¿¹cy wierzcho³ek jako odwiedzony
    for(u = 0; u < n; u++)        // Przegl¹damy s¹siadów wierzcho³ka v
      if(A[v][u] && !visited[u])  // Szukamy nieodwiedzonego jeszcze s¹siada
      {
        dh += W[v][u];            // Dodajemy wagê krawêdzi v-u do sumy
        TSP(u);                   // Rekurencyjnie wywo³ujemy szukanie cyklu Hamiltona
        dh -= W[v][u];            // Usuwamy wagê krawêdzi z sumy
      }
    visited[v] = false;           // Zwalniamy bie¿¹cy wierzcho³ek
  }
  else if(A[v0][v])               // Jeœli znaleziona œcie¿ka jest cyklem Hamiltona
  {
    dh += W[v][v0];               // to sprawdzamy, czy ma najmniejsz¹ sumê wag
    if(dh < d)                    // Jeœli tak,
    {
      d = dh;                     // To zapamiêtujemy tê sumê
      for(u = 0; u < shptr; u++)  // oraz kopiujemy stos Sh do S
        S[u] = Sh[u];
      sptr = shptr;
    }
    dh -= W[v][v0];               // Usuwamy wagê krawêdzi v-v0 z sumy
  }
  shptr--;                        // Usuwamy bie¿¹cy wierzcho³ek ze œcie¿ki
}

*/

void Graph::DFS(Node *node) {
	stack.push(node);
	if (toString(getPath()).length() > length) {
		stack.pop();
		return;
	}
	//printf("%s\n", toString(getPath()).c_str());
	if (toString(getPath()).length() < length) {
		visited[node]--;
		for (auto e : node->edges) {
			if (visited[e->dest()] > 0) {
				sum += e->weight();
				DFS(e->dest());
				sum -= e->weight();
			}
		}
		visited[node]++;
	}
	/*
	else if ((toString(getPath()).length() == length) && (containsAllOligos()))
	{
		//printf("%s : %d\n", toString(getPath()).c_str(), sum);
		if (sum < maxSum) {
			maxSum = sum;
			finalStack = stack;
		}
		//getPath();
		//return;
	}
	*/
	
	stack.pop();
}