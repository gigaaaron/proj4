# Overview
Abstract base class for path router
### TVertexID
Public type alias for std::size_t. This is used to identify vertices in the graph.
### InvalidVertexID
Used to denote a vertex that is invalid or does not exist.
### NoPathExists
Used to denote unreachable paths

## Functions
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
