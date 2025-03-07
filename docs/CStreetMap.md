# Overview
Interface for the street map.

## SNode
Street node object that represents locations in the street map.
### Constructor
Initializes instance of street node.
#### Parameters
* TNodeID: ID for the node.
* double: Latitude coordinate.
* double: Longitude coordinate
### ID
Returns ID of node.
### Location
Returns location as coordinates.
### AttributeCount
Returns number of node attributes.
### GetAttributeKey
Returns key of node attribute at given index.
### HasAttribute
Returns bool if attribute with given key exists or not.
### GetAttribute
Returns value of node attribute with given key.

## SWay
Street way object that connects nodes in the street map.
### Constructor
Initializes instance of street way.
#### Parameters
* TWayID: ID for the way.
### NodeCount
Returns number of nodes in way.
### GetNodeID
Returns ID of node at given index.
### AttributeCount
Returns number of way attributes.
### GetAttributeKey
Returns key of way attribute at given index.
### HasAttribute
Returns bool if attribute with given key exists or not.
### GetAttribute
Returns value of way attribute with given key.

## NodeCount
See COpenStreetMap.md for documentation.

## WayCount
See COpenStreetMap.md for documentation.

## NodeByIndex
See COpenStreetMap.md for documentation.

## NodeByID
See COpenStreetMap.md for documentation.

## WayByIndex
See COpenStreetMap.md for documentation.

## WayByID
See COpenStreetMap.md for documentation.
