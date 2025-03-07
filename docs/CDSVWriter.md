# Overview
The reader will parse one row of a DSV file into a vector of strings.

## Constructor
The constructor creates a DSVReader object that parses the DSV.
### Parameters
* std::shared_ptr< CDataSource >: The data source the object reads from.
* char: The delimiter separating values in the DSV file.

## End
The function checks if the DSVReader object has reached the end of the DSV file.
### Return
bool: The function returns true if the end has been reached and false if it has not.

## ReadRow
The function separates a row from the DSV file by searching for an unquoted newline character. It then splits the row along unquoted delimiter characters.
### Parameters
* std::vector< std::string >&: The address of the vector to store the processed row.
### Return
bool: The function returns true if the row has been successfully read and false if not.