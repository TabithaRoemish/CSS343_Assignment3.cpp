#include <queue>
#include <climits>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <stack>


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
	vertices.clear(); //probably don't need
}                                                         

/** destructor, delete all vertices and edges
only vertices stored in map
no pointers to edges created by graph */
Graph::~Graph() 
{ 
	// go through graph verticies and delete
	for (std::map<std::string, Vertex*>::iterator it = vertices.begin(),
		it_end = vertices.end(); it != it_end; it++)
	{
		delete it->second;
	}
	
}																	

/** return number of vertices */
int Graph::getNumVertices() const { return numberOfVertices; }

/** return number of vertices */
int Graph::getNumEdges() const { return numberOfEdges; }

/** add a new edge between start and end vertex
if the vertices do not exist, create them
calls Vertex::connect
a vertex cannot connect to itself
or have multiple edges to another vertex */
bool Graph::add(std::string start, std::string end, int edgeWeight) 
{ 
	findOrCreateVertex(end);
	Vertex * temp = findOrCreateVertex(start);
	
	
	if ((temp->connect(end, edgeWeight)))
	{
		numberOfEdges++;
		return true;
	}
	else
		return false; 
	
}																					

/** return weight of the edge between start and end
returns INT_MAX if not connected or vertices don't exist */
int Graph::getEdgeWeight(std::string start, std::string end) const 
{ 
	return vertices.at(start)->getEdgeWeight(end);
} 

/** read edges from file
the first line of the file is an integer, indicating number of edges
each edge line is in the form of "string string int"
fromVertex  toVertex    edgeWeight */
void Graph::readFile(std::string filename) 
{
	std::ifstream toRead(filename);
	int weight = 0;
	
	std::string parse = " ";
	std::string startVertex = " ";
	std::string endVertex = " ";
	std::stringstream ss;

	if (toRead.is_open())
	{
		std::getline(toRead, parse);
		parse = " ";
		while (std::getline(toRead, parse))
		{
			ss << parse;
			ss >> startVertex >> endVertex >> weight;

			/*findOrCreateVertex(startVertex);
			findOrCreateVertex(endVertex);*/
			add(startVertex, endVertex, weight);

			//reset
			startVertex = " ";
			endVertex = " ";
			parse = " ";
			ss.clear();
		}

	}
	else
		std::cerr << "Could not open file: " << filename;
	
	toRead.close();
	
}														
/** depth-first traversal starting from startLabel
call the function visit on each vertex label */
void Graph::depthFirstTraversal(std::string startLabel,
	void visit(const std::string&)) 
{
	std::stack<Vertex> mystack;
	std::string current = " ";
	this->unvisitVertices();

	mystack.push(*(vertices.at(startLabel)));
	visit(vertices.at(startLabel)->getLabel());
	vertices.at(startLabel)->visit();

	while (!mystack.empty())
	{
		//find unvisited neighbor
		current = findUnvisitedNeighbor(mystack.top().getLabel());
		
		if (!(vertices.at(current)->isVisited()))
		{
			mystack.push(*(vertices.at(current)));
			vertices.at(current)->visit();
			visit(current);
		}
		else
			mystack.pop();
	}
	
}                        
std::string Graph::findUnvisitedNeighbor(std::string startVertex)
{
	std::string unvisitedneighbor = vertices.at(startVertex)->getNextNeighbor();
	int neighbors = vertices.at(startVertex)->getNumberOfNeighbors();
	for (int i = 0; i < neighbors; i++)
	{
		if (!(vertices.at(unvisitedneighbor)->isVisited()))
			break;
		else
			unvisitedneighbor = vertices.at(startVertex)->getNextNeighbor();
			
	}
	return unvisitedneighbor;
}
/** breadth-first traversal starting from startLabel
call the function visit on each vertex label */
void Graph::breadthFirstTraversal(std::string startLabel,      
	void visit(const std::string&)) 
{
	std::queue<Vertex> myqueue;
	std::string neighbor;
	std::string currentVertex = startLabel;
	int size = 0;

	this->unvisitVertices();
	myqueue.push(*(vertices.at(startLabel)));
	vertices.at(startLabel)->visit();
	visit(startLabel);
	
	while (!myqueue.empty())
	{
		currentVertex = myqueue.front().getLabel();
		size = vertices.at(currentVertex)->getNumberOfNeighbors();
		myqueue.pop();
		for (int i = 0; i < size; i++)
		{
			neighbor = vertices.at(currentVertex)->getNextNeighbor();
			if (!vertices.at(neighbor)->isVisited())
			{
				visit(neighbor);
				vertices.at(neighbor)->visit();
				myqueue.push(*(vertices.at(neighbor)));
			}
		}	
	}
}

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
	std::map<std::string, std::string>& previous) 
{
	this->unvisitVertices();
	std::string u = " ";
	std::string v = " ";
	int numOfNeighbors = vertices.at(startLabel)->getNumberOfNeighbors();
	int vNeighbors = 0;
	int v2ucost = 0;
	weight.clear();
	previous.clear();

	typedef std::pair<int, std::string> P;
	struct Order
	{
		bool operator()(P a, P b) const
		{
			return a.first > b.first;
		}
	};
	std::priority_queue<P,std::deque<P>, Order> pq;
	std::set<std::string> vertexSet;
	
	for (int i = 0; i < numOfNeighbors; i++) // set initial weights
	{
		u = vertices.at(startLabel)->getNextNeighbor();
		weight[u] = vertices.at(startLabel)->getEdgeWeight(u);
		previous[u] = startLabel;
		pq.push(P(weight[u], u));
	}
	vertexSet.insert((vertices.at(startLabel)->getLabel()));
	while (!pq.empty())
	{
		v = pq.top().second;// lowest cost, first in priority queue
		vertexSet.insert((vertices.at(v)->getLabel()));
		pq.pop();
		if (vertexSet.find(v) != vertexSet.end())
		{
			vNeighbors = vertices.at(v)->getNumberOfNeighbors();
			u = vertices.at(v)->getNextNeighbor();
			for (int k = 0; k < vNeighbors; k++)
			{
				if (vertexSet.count(u) > 0)
				{
					u = vertices.at(v)->getNextNeighbor();
					continue;
				}
				v2ucost = vertices.at(v)->getEdgeWeight(u);
				if (weight.find(u) == weight.end())
				{
					weight[u] = weight[v] + v2ucost;
					previous[u] = v;
					pq.push(P(weight[u], u));
				}
				else
				{
					if (weight[u] > (weight[v] + v2ucost))
					{
						weight[u] = weight[v] + v2ucost;
						previous[u] = v;
						pq.push(P(weight[u], u));
					}
				}
				u = vertices.at(v)->getNextNeighbor();
			}
		}
	}
}															

///** helper for depthFirstTraversal */
//void Graph::depthFirstTraversalHelper(Vertex* startVertex,  //not used
//	void visit(const std::string&)) 
//{
//
//}

///** helper for breadthFirstTraversal */
//void Graph::breadthFirstTraversalHelper(Vertex*startVertex,  //not used
//	void visit(const std::string&)) {}

/** mark all verticies as unvisited */
void Graph::unvisitVertices() 
{
	std::map<std::string, Vertex*>::iterator itr = vertices.begin();
	while (itr != vertices.end())
	{
		itr->second->unvisit();
		itr->second->resetNeighbor();
		itr++;
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
Vertex* Graph::findOrCreateVertex(const std::string& vertexLabel) 

{ 
	if (findVertex(vertexLabel) == nullptr)
	{
		Vertex * newVertex = new Vertex(vertexLabel);
		vertices[vertexLabel] = newVertex;
		vertices.at(newVertex->getLabel())->resetNeighbor();
		numberOfVertices++;
		return newVertex;
	}
	else
		return findVertex(vertexLabel);
	
}	

bool Graph::removeEdge(std::string start, std::string end)
{
	return vertices.at(start)->disconnect(end);
}

