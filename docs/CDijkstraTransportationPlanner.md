# Overview
Implementation of TransportPlanner abstract class
### Parameters
Pointer to an instance of SConfiguration
### Constructor
Creates planner using provided configuration

## Functions
### NodeCount
Returns the number of nodes in the street map.
### SortedNodeByIndex
Returns a shared pointer to the node at given index.
### FindShortestPath
Returns the shortest path from source to destination in the form of a vector of nodes IDs.
### FindFastestPath
Returns the fastest path from source to destintation in the form of a vector of TTripStep pairs.
### GetPathDescription
Converts a path to human-readable descriptions.

