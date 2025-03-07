# Overview
Implementation of the bus system.

## Constructor
Creates an instance of the bus system and populates it with stops and routes.
## Parameters
* std::shared_ptr< CDSVReader > stopsrc: Data source for stops.
* std::shared_ptr< CDSVReader > routesrc: Data source for routes.

## StopCount
Returns number of stops in the bus system.
### Return
std::size_t: Number of stops in the bus system.

## RouteCount
Returns number of routes in the bus system.
### Return
std::size_t: Number of routes in the bus system.

## StopByIndex
Returns pointer to stop at index.
### Parameters
* std::size_t index: Index of the target stop.
### Return
* std::shared_ptr< SStop >: Pointer to the target stop.
* nullptr: Pointer if index is invalid.

## StopByID
Returns pointer to stop with ID.
### Parameters
* TStopID: ID of the target stop.
### Return
* std::shared_ptr< SStop >: Pointer to the target stop.
* nullptr: Pointer if ID is invalid.

## RouteByIndex
Returns pointer to route at index.
### Parameters
* std::size_t index: Index of the target route.
### Return
* std::shared_ptr< SRoute >: Pointer to the target route.
* nullptr: Pointer if index is invalid.

## RouteByName
Returns pointer to route with name.
### Parameters
* std::string: Name of the target route.
### Return
* std::shared_ptr< SRoute >: Pointer to the target route.
* nullptr: Pointer if name is invalid.
