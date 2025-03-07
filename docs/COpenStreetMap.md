# Overview
Implementation of the street map.

## Constructor
Creates an instance of the street map and populates it with nodes and ways.
## Parameters
std::shared_ptr< CXMLReader > src: Data source for the street map's nodes and ways.

## NodeCount
Returns number of nodes in the street map.
### Return
* std::size_t: Number of nodes in the street map.

## WayCount
Returns number of ways in the street map.
### Return
* std::size_t: Number of ways in the street map.

## NodeByIndex
Returns pointer to node at given index.
### Parameters
* std::size_t index: Index of the target node.
### Return
* std::shared_ptr< SNode >: Pointer to the target node.
* nullptr: Pointer if index is invalid.

## NodeByID
Returns pointer to node with ID.
### Parameters
* TNodeID: ID of the target node.
### Return
* std::shared_ptr< SNode >: Pointer to the target node.
* nullptr: Pointer if ID is invalid.

## WayByIndex
Returns pointer to way at index.
### Parameters
std::size_t index: Index of the target way.
### Return
* std::shared_ptr< SWay >: Pointer to the target way.
* nullptr: Pointer if index is invalid.

## WayByID
Returns pointer to way with ID.
### Parameters
* TWayID: ID of the target way.
### Return
* std::shared_ptr< SWay >: Pointer to the target way.
* nullptr: Pointer if ID is invalid.
