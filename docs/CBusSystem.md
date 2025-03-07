# Overview
Interface for the bus system.

## SStop
Stop object that populates the bus system.
#### Constructor
Initializes an instance of the stop.
#### Parameters
* TStopID s: ID for the stop.
* CStreetMap::TNodeID: ID for the node.
### ID
Returns stop ID.
### NodeID
Returns node ID of the stop.

## SRoute
Route object that connects stops in the bus system.
### Constructor
Initializes an instance of the route.
#### Parameters
* std::string n: Name for the route.
* TStopID s: ID of the first stop in the route.
### Name
Returns route name.
### StopCount
Returns number of stops in the route.
### GetStopID
Returns ID of stop at index.

## StopCount
See CCSVBusSystem.md for documentation.

## RouteCount
See CCSVBusSystem.md for documentation.

## StopByIndex
See CCSVBusSystem.md for documentation.

## StopByID
See CCSVBusSystem.md for documentation.

## RouteByIndex
See CCSVBusSystem.md for documentation.

## RouteByName
See CCSVBusSystem.md for documentation.
