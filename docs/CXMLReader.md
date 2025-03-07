# Overview
The reader will parse an XML file into SXMLEntity objects.

## Constructor
The constructor creates an XMLReader object that reads the XML file.
### Parameters
* std::shared_ptr< CDataSource >: The data source the object reads from.

## End
The function checks if the XMLReader object has reached the end of the XML file.
### Return
bool: The function returns true if the end has been reached and false if it has not.

## ReadEntity
The function separates and returns an entity from the XML file as an SXMLEntity object.
### Parameters
* SXMLEntity &entity: The address of the object to store the processed entity.
* bool: If the character data should be ignored; false by default.
### Return
bool: The function returns true if the entity has been successfully processed and false if not.