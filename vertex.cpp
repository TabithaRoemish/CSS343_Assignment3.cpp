#include <climits>

#include "vertex.h"


#include <functional>
#include <map>
#include <string>

#include "edge.h"


////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////


/** Creates an unvisited vertex, gives it a label, and clears its
adjacency list.
NOTE: A vertex must have a unique label that cannot be changed. */
Vertex::Vertex(std::string label) 
{
	//check all verticies to make sure lable is unique //<-----------------------Still need to do
	visited = false;
	vertexLabel = label;

}

/** @return  The label of this vertex. */
std::string Vertex::getLabel() const { return vertexLabel; }

/** Marks this vertex as visited. */
void Vertex::visit() { visited = true; }

/** Marks this vertex as not visited. */
void Vertex::unvisit() { visited = false; }

/** Returns the visited status of this vertex.
@return  True if the vertex has been visited, otherwise
returns false/ */
bool Vertex::isVisited() const { return visited; }

/** Adds an edge between this vertex and the given vertex.
Cannot have multiple connections to the same endVertex
Cannot connect back to itself
@return  True if the connection is successful. */
bool Vertex::connect(const std::string& endVertex, const int edgeWeight) { return true; }//<-----------------------Still need to do

/** Removes the edge between this vertex and the given one.
@return  True if the removal is successful. */
bool Vertex::disconnect(const std::string& endVertex) { return true; } //<-----------------------Still need to do

/** Gets the weight of the edge between this vertex and the given vertex.
@return  The edge weight. This value is zero for an unweighted graph and
is negative if the .edge does not exist */
int Vertex::getEdgeWeight(const std::string& endVertex) const 
{ 
	if(adjacencyList.find(endVertex) != adjacencyList.end())
		//if graph is unweighted, the value will be zero here
		return adjacencyList.at(endVertex).getWeight(); 
	else 
		return -1; 
}

/** Calculates how many neighbors this vertex has.
@return  The number of the vertex's neighbors. */
int Vertex::getNumberOfNeighbors() const { return adjacencyList.size(); }

/** Sets current neighbor to first in adjacency list. */
void Vertex::resetNeighbor() { currentNeighbor = adjacencyList.begin(); }

/** Gets this vertex's next neighbor in the adjacency list.
Neighbors are automatically sorted alphabetically via map
Returns the vertex label if there are no more neighbors
@return  The label of the vertex's next neighbor. */
std::string Vertex::getNextNeighbor() 
{ 
	std::map<std::string, Edge>::iterator end = adjacencyList.end();
	end--;

	if (currentNeighbor == end)
		return end->first; // return vertex label if no more neighbors
	else
		currentNeighbor++;
	
	return currentNeighbor->first;
}

/** Sees whether this vertex is equal to another one.
Two vertices are equal if they have the same label. */
bool Vertex::operator==(const Vertex& rightHandItem) const 
{ return this->getLabel() == rightHandItem.getLabel(); }

/** Sees whether this vertex is < another one.
Compares vertexLabel. */
bool Vertex::operator<(const Vertex& rightHandItem) const 
{ return this->getLabel()< rightHandItem.getLabel(); }
