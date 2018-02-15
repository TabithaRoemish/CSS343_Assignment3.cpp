//FileName: ass3.cpp
//Additional function testing by Tabitha Roemish
//Date: February 1, 2018

#include <iostream>
#include <climits>
#include <map>
#include <sstream>
#include <vector>

#include "graph.h"

////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////

using namespace std;

// global variable - need better method, but works for testing
// used by graphVisitor
ostringstream graphOut;

// global variable for Djikstra
map<string, int> weight;
map<string, string> previous;

// OK or ERR for tests
template <typename T>
string isOK(const T& got, const T& expected) {
	if (got == expected) {
		return "OK: ";
	}
	else {
		cout << "    Got   " << got << "\n expected " << expected << endl;
		return "ERR: ";
	}
}

// visitor function - print out the vertex label
void display(const string& anItem) {
	cout << anItem << " ";
}

// visitor function - add the string to global variable graphOut
void graphVisitor(const string& anItem) {
	graphOut << anItem << " ";
}

// add the path to get to this vertex to global variable global variable
// [A B]
// previous is a map of vertexLabel prevVertexLabel
// need to process it in reverse to get the path
void graphCostDisplayPath(string vertex) {
	vector<string> v;
	auto prevIt = previous.find(vertex);
	while (prevIt != previous.end()) {
		v.push_back(prevIt->second);
		prevIt = previous.find(prevIt->second);
	}
	// v now has the path
	// oh, avoiding the joy of subtracting from unsigned int!
	int size = static_cast<int>(v.size());
	// return if we did we not go through any other vertex
	if (size <= 1) {
		return;
	}
	// add space between entries
	graphOut << "via [";
	// all entries except the first and last one in reverse
	// v[size-1] is the starting vertex, skipping it
	for (int i{ size - 2 }; i > 0; --i)
		graphOut << v[i] << " ";
	// add the second to last entry without space after it
	// last entry is the destination vertex, not needed
	if (size > 0)
		graphOut << v[0];
	graphOut << "] ";
}

// unreachable nodes should not even be in weight
// but if they are,
// skip them as they should have weight of INT_MAX
// C(8) via [A B]
// getting to C has a cost of 8, we can get to C via A->B->C
void graphCostDisplay() {
	graphOut.str("");
	for (auto it : weight) {
		int cost = it.second;
		if (cost == INT_MAX)
			continue;
		string vertex = it.first;
		graphOut << vertex << "(" << cost << ") ";
		graphCostDisplayPath(vertex);
	}
}

void testGraph0() {
	cout << "testGraph0" << endl;
	Graph g;
	g.readFile("graph0.txt");
	cout << isOK(g.getNumVertices(), 3) << "3 vertices" << endl;
	cout << isOK(g.getNumEdges(), 3) << "3 edges" << endl;

	graphOut.str("");
	g.depthFirstTraversal("A", graphVisitor);
	cout << isOK(graphOut.str(), "A B C "s) << "DFS" << endl;

	graphOut.str("");
	g.breadthFirstTraversal("A", graphVisitor);
	cout << isOK(graphOut.str(), "A B C "s) << "BFS" << endl;

	g.djikstraCostToAllVertices("A", weight, previous);
	graphCostDisplay();
	cout << isOK(graphOut.str(), "B(1) C(4) via [B] "s)
		<< "Djisktra" << endl;
}

void testGraph1() {
	cout << endl;
	cout << "testGraph1" << endl;
	Graph g;
	g.readFile("graph1.txt");
	cout << isOK(g.getNumVertices(), 10) << "10 vertices" << endl;
	cout << isOK(g.getNumEdges(), 9) << "9 edges" << endl;

	graphOut.str("");
	g.depthFirstTraversal("A", graphVisitor);
	cout << isOK(graphOut.str(), "A B C D E F G H "s) << "DFS" << endl;

	graphOut.str("");
	g.breadthFirstTraversal("A", graphVisitor);
	cout << isOK(graphOut.str(), "A B H C G D E F "s) << "BFS" << endl;

	g.djikstraCostToAllVertices("A", weight, previous);
	graphCostDisplay();
	cout << isOK(graphOut.str(),
		"B(1) C(2) via [B] "s +
		"D(3) via [B C] E(4) via [B C D] "s +
		"F(5) via [B C D E] " +
		"G(4) via [H] "s +
		"H(3) "s)
		<< "Djisktra" << endl;
}

void testGraph2() {
	cout << endl;
	cout << "testGraph2" << endl;
	Graph g;
	g.readFile("graph2.txt");
	cout << isOK(g.getNumVertices(), 21) << "21 vertices" << endl;
	cout << isOK(g.getNumEdges(), 24) << "24 edges" << endl;

	graphOut.str("");
	g.depthFirstTraversal("A", graphVisitor);
	cout << isOK(graphOut.str(), "A B E F J C G K L D H M I N "s)
		<< "DFS from A" << endl;

	graphOut.str("");
	g.depthFirstTraversal("O", graphVisitor);
	cout << isOK(graphOut.str(), "O P R S T U Q "s)
		<< "DFS from O" << endl;

	graphOut.str("");
	g.breadthFirstTraversal("A", graphVisitor);
	cout << isOK(graphOut.str(), "A B C D E F G H I J K L M N "s)
		<< "BFS from A" << endl;

	graphOut.str("");
	g.breadthFirstTraversal("D", graphVisitor);
	cout << isOK(graphOut.str(), "D H I M N "s)
		<< "BFS from D" << endl;

	graphOut.str("");
	g.depthFirstTraversal("U", graphVisitor);
	cout << isOK(graphOut.str(), "U "s)
		<< "DFS from U" << endl;

	graphOut.str("");
	g.breadthFirstTraversal("U", graphVisitor);
	cout << isOK(graphOut.str(), "U "s)
		<< "BFS from U" << endl;

	g.djikstraCostToAllVertices("O", weight, previous);
	graphCostDisplay();
	cout << isOK(graphOut.str(),
		"P(5) Q(2) R(3) via [Q] S(6) via [Q R] " +
		"T(8) via [Q R S] U(9) via [Q R S] "s)
		<< "Djisktra O" << endl;
}

void testGraph3()
{
	cout << endl;
	cout << "testGraph3" << endl;
	Graph g;
	g.add("A", "B", 5);
	g.add("B", "C", 6);
	g.add("A", "C", 3);
	g.add("C", "D", 8);
	g.add("C", "E", 1);

	cout << isOK(g.getNumVertices(), 5) << "5 vertices" << endl;
	cout << isOK(g.getNumEdges(), 5) << "5 edges" << endl;

	graphOut.str("");
	g.depthFirstTraversal("A", graphVisitor);
	cout << isOK(graphOut.str(), "A B C D E "s)
		<< "DFS from A" << endl;
	
	graphOut.str("");
	g.breadthFirstTraversal("A", graphVisitor);
	cout << isOK(graphOut.str(), "A B C D E "s)
		<< "BFS from A" << endl;

	g.djikstraCostToAllVertices("A", weight, previous);
	graphCostDisplay();
	cout << isOK(graphOut.str(),
		"B(5) C(3) D(11) via [C] E(4) via [C] "s)
		<< "Djisktra" << endl;
	
	g.add("B", "D", 1);
	g.add("B", "X", 2);

	cout << isOK(g.getNumVertices(), 6) << "6 vertices" << endl;
	cout << isOK(g.getNumEdges(), 7) << "7 edges" << endl;

	graphOut.str("");
	g.depthFirstTraversal("A", graphVisitor);
	cout << isOK(graphOut.str(), "A B C D E X "s)
		<< "DFS from A" << endl;

	graphOut.str("");
	g.breadthFirstTraversal("A", graphVisitor);
	cout << isOK(graphOut.str(), "A B C D X E "s)
		<< "BFS from A" << endl;

	g.djikstraCostToAllVertices("A", weight, previous);
	graphCostDisplay();
	cout << isOK(graphOut.str(),
		"B(5) C(3) D(6) via [B] E(4) via [C] X(7) via [B] "s)
		<< "Djisktra" << endl;

	g.removeEdge("A", "B");

	cout << isOK(g.getNumVertices(), 6) << "6 vertices" << endl;
	cout << isOK(g.getNumEdges(), 6) << "6 edges" << endl;

	graphOut.str("");
	g.depthFirstTraversal("A", graphVisitor);
	cout << isOK(graphOut.str(), "A C D E "s)
		<< "DFS from A" << endl;

	graphOut.str("");
	g.breadthFirstTraversal("A", graphVisitor);
	cout << isOK(graphOut.str(), "A C D E "s)
		<< "BFS from A" << endl;

}

void testGraph4()
{
	cout << endl;
	cout << "test4 - graph3.txt" << endl;
	Graph g;
	g.readFile("graph3.txt");
	cout << isOK(g.getNumVertices(), 22) << "22 vertices" << endl;
	cout << isOK(g.getNumEdges(), 26) << "26 edges" << endl;

}

void testGraph5()
{
	cout << endl;
	cout << "test5 - graph2.txt with add" << endl;
	Graph g;
	g.add("A", "B", 0);
	g.add("A", "C", 0);
	g.add("A", "D", 0);
	g.add("B", "E", 0);
	g.add("B", "F", 0);
	g.add("C", "G", 0);
	g.add("D", "H", 0);
	g.add("D", "I", 0);
	g.add("F", "J", 0);
	g.add("G", "K", 0);
	g.add("G", "L", 0);
	g.add("H", "M", 0);
	g.add("I", "M", 0);
	g.add("I", "N", 0);
	g.add("O", "P", 5);
	g.add("O", "Q", 2);
	g.add("P", "R", 2);
	g.add("Q", "R", 1);
	g.add("R", "O", 1);
	g.add("R", "S", 3);
	g.add("S", "R", 1);
	g.add("S", "T", 2);
	g.add("S", "U", 3);
	g.add("T", "O", 8);
	
	cout << isOK(g.getNumVertices(), 21) << "21 vertices" << endl;
	cout << isOK(g.getNumEdges(), 24) << "24 edges" << endl;

}
int main() {
	testGraph0();
	testGraph1();
	testGraph2();
	testGraph3();
	testGraph4();
	testGraph5();
	return 0;
}