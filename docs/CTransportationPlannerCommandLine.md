# Overview
Allows for users to interact with the CTransportPlanner through a command-line interface.
### Constructor
Initializes the Commandline with the necessary sinks.
#### cmdsrc
The data source for providing the commands for the planner.
#### outsink 
The data sink used for output.
#### errsink
The data sink used for error output.
#### results 
The data factory used to generate and process results from the planner.
#### planner
The transportation planner to process the commands.

## Functions
### ProcessCommands
Processes commands received from cmdsrc and produces the output on outsink, while writing error to errsink.
Function returns true if commands were successfully executed and false if there is an error.
