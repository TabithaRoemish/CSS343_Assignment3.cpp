#include <queue>
#include <climits>
#include <set>
#include <iostream>
#include <fstream>
#include <map>

#include "graph.h"

/**
* A graph is made up of vertices and edges
* A vertex can be connected to other vertices via weighted, directed edge
*/


////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////


/** constructor, empty graph */
Graph::Graph() 
{
	numberOfEdges = 0;
	numberOfVertices = 0;
	vertices.clear();
}                                                         //<-----------------------Still need to do

/** destructor, delete all vertices and edges
only vertices stored in map
no pointers to edges created by graph */
Graph::~Graph() {}                                                         //<-----------------------Still need to do

/** return number of vertices */
int Graph::getNumVertices() const { return numberOfVertices; }

/** return number of vertices */
int Graph::getNumEdges() const { return numberOfEdges; }

/** add a new edge between start and end vertex
if the vertices do not exist, create them
calls Vertex::connect
a vertex cannot connect to itself
or have multiple edges to another vertex */
bool Graph::add(std::string start, std::string end, int edgeWeight) { return true; } //<-----------------------Still need to do

/** return weight of the edge between start and end
returns INT_MAX if not connected or vertices don't exist */
int Graph::getEdgeWeight(std::string start, std::string end) const { return 0; } //<-----------------------Still need to do

/** read edges from file
the first line of the file is an integer, indicating number of edges
each edge line is in the form of "string string int"
fromVertex  toVertex    edgeWeight */
void Graph::readFile(std::string filename) {}                  //<-----------------------Still need to do

/** depth-first traversal starting from startLabel
call the function visit on each vertex label */
void Graph::depthFirstTraversal(std::string startLabel,
	void visit(const std::string&)) {}                         //<-----------------------Still need to do

/** breadth-first traversal starting from startLabel
call the function visit on each vertex label */
void Graph::breadthFirstTraversal(std::string startLabel,      //<-----------------------Still need to do
	void visit(const std::string&)) {}

/** find the lowest cost from startLabel to all vertices that can be reached
using Djikstra's shortest-path algorithm
record costs in the given map weight
weight["F"] = 10 indicates the cost to get to "F" is 10
record the shortest path to each vertex using given map previous
previous["F"] = "C" indicates get to "F" via "C"

cpplint gives warning to use pointer instead of a non-const map
which I am ignoring for readability */
void Graph::djikstraCostToAllVertices(
	std::string startLabel,
	std::map<std::string, int>& weight,
	std::map<std::string, std::string>& previous) {}       //<-----------------------Still need to do

/** helper for depthFirstTraversal */
void Graph::depthFirstTraversalHelper(Vertex* startVertex,  //<-----------------------Still need to do
	void visit(const std::string&)) {}

/** helper for breadthFirstTraversal */
void Graph::breadthFirstTraversalHelper(Vertex*startVertex,  //<-----------------------Still need to do
	void visit(const std::string&)) {}

/** mark all verticies as unvisited */
void Graph::unvisitVertices() 
{
	std::map<std::string, Vertex*>::iterator itr = vertices.begin();
	while (itr != vertices.end())
	{
		itr->second->unvisit();
	}
}

/** find a vertex, if it does not exist return nullptr */
Vertex* Graph::findVertex(const std::string& vertexLabel) const 
{ 
	if (vertices.find(vertexLabel) != vertices.end())
		return vertices.at(vertexLabel);
	else
		return nullptr; 
}

/** find a vertex, if it does not exist create it and return it */
Vertex* Graph::findOrCreateVertex(const std::string& vertexLabel) { return nullptr; }  //<-----------------------Still need to do