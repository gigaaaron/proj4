# Overview
Route planning class that takes into account for bus/walk and biking.
### TNodeID
Represents the ID of a node on the street map.
### ETransportMode
Used to represent the modes of transportation: walking, biking, and bussing.
### TTripStep
Defines the step of the trip along with the mode of transportation.

## SConfiguration
### StreetMap
Returns a shared pointer to the street map.
### BusSystem
Returns a shared pointer to the bus system.
### WalkSpeed
Returns the walking speed in units per time.
### BikeSpeed
Returns the biking speed in units per time.
### DefaultSpeedLimit
Returns the default speed limit.
### BusStopTime
Returns the time spent at a bus stop.
### PrecomputeTime
Returns the pre-computation time.

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

