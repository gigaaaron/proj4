# Overview
The writer writes an XMLEntity object into a formatted XML file.

## Constructor
The constructor creates a XMLWriter object that writes the XML entity.
### Parameters
* std::shared_ptr< CDataSink >: The data sink the object writes to.

## WriteEntity
The function takes an entity and writes it in XML format.
### Parameters
* const SXMLEntity &: The SXMLEntity object to write.
### Return
bool: The function returns true if the entity has been successfully written and false if not.
