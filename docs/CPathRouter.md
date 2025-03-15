# Overview
Interface for path router

## Constructor
Initalize an instance of CDijkstraPathRouter class
## Vertex
Respresents a vertex in a graph. It stores a tag and a map of all adjacent edges with associated weights
## VertexID
Stored as TVertexID.
## Edge Weight
Represented by a double.
## Bidirectional Edges
When bidir is set to true, one edge is added for each direction.

## Methods
### VertexCount
Returns the number of vertices in the graph
### AddVertex
Adds vertex to the graph with a tag. Returns the ID of the added vertex
### GetAnyVertexTag
Returns the tag of a vertex
### AddEdge
Returns true if an edge is successfully and false otherwise.
### Precompute
Allows path router to do precomputations up to a given deadline
### FindShortestPath
Returns the sum of the edges' weight that construct the shortest path
