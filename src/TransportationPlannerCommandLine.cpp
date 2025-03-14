#include "TransportationPlannerCommandLine.h"
#include "DijkstraTransportationPlanner.h"
#include "OpenStreetMap.h"
#include "DSVReader.h"
#include "DSVWriter.h"
#include "StringUtils.h"
#include <memory>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

struct CTransportationPlannerCommandLine::SImplementation
{
	std::shared_ptr<CDataSource> cmdsrc;
	std::shared_ptr<CDataSink> outsink;
	std::shared_ptr<CDataSink> errsink;
	std::shared_ptr<CDataFactory> results;
	std::shared_ptr<CTransportationPlanner> planner;

	std::vector<CTransportationPlanner::TTripStep> path;
	double pathtime;

	SImplementation(std::shared_ptr<CDataSource> c, std::shared_ptr<CDataSink> o, std::shared_ptr<CDataSink> e, std::shared_ptr<CDataFactory> r, std::shared_ptr<CTransportationPlanner> p)
	{
		cmdsrc = c;
		outsink = o;
		errsink = e;
		results = r;
		planner = p;
	}

	bool ProcessCommands()
	{
		while (1)
		{
			outsink->Write(Format("> "));

			// Get input
			char c;
			std::string input;
			while (c != '\n')
			{
				input += c;
				cmdsrc->Get(c);
			}

			std::vector<std::string> command = StringUtils::Split(input, " ");

			if (command[0] == "help")	
				Help();

			else if (command[0] == "exit")
				break;

			else if (command[0] == "count")
				outsink->Write(Format(std::to_string(planner->NodeCount())));

			else if (command[0] == "node")
				Node(command);
			
			else if (command[0] == "fastest")
				Fastest(command);
			
			else if (command[0] == "shortest")
				Shortest(command);
			
			else if (command[0] == "save")
				Save();
			
			else if (command[0] == "print")
				Print();
			
			else
				errsink->Write(Format("Unknown command \"" + command[0] + "\" type help for help.\n"));
		}

		return true;
	}

	void Help() {
		outsink->Write(Format("------------------------------------------------------------------------\n"));
		outsink->Write(Format("help     Display this help menu\n"));
		outsink->Write(Format("exit     Exit the program\n"));
		outsink->Write(Format("count    Output the number of nodes in the map\n"));
		outsink->Write(Format("node     Syntax \"node [0, count)\" \n"));
		outsink->Write(Format("         Will output node ID and Lat/Lon for node\n"));
		outsink->Write(Format("fastest  Syntax \"fastest start end\" \n"));
		outsink->Write(Format("         Calculates the time for fastest path from start to end\n"));
		outsink->Write(Format("shortest Syntax \"shortest start end\" \n"));
		outsink->Write(Format("         Calculates the distance for the shortest path from start to end\n"));
		outsink->Write(Format("save     Saves the last calculated path to file\n"));
		outsink->Write(Format("print    Prints the steps for the last calculated path\n"));
	}

	void Node(std::vector<std::string> command) {
		if (command.size() < 2) 
		{
			errsink->Write(Format("Invalid node command, see help.\n"));
			return;
		}
		
		// Check if index is a number
		int index;
		try 
		{
			index = std::stoi(command[1]);
		} 
		catch (std::invalid_argument &excpt)
		{
			errsink->Write(Format("Invalid node parameter, see help.\n"));
			return;
		}
		
		// Check if index falls within range
		if (0 <= index < planner->NodeCount()) {
			auto node = planner->SortedNodeByIndex(index);
			auto id = std::to_string(node->ID());
			auto coordinates = Coordinate(node->Location().first) + ", " + Coordinate(node->Location().second, false);

			outsink->Write(Format("Node " + std::to_string(index) + ": id = " + id + " is at " + coordinates + "\n"));
		} else {
			errsink->Write(Format("Invalid node parameter, see help.\n"));
			return;
		}
	}

	void Fastest(std::vector<std::string> command)
	{
		if (command.size() < 3) {
			errsink->Write(Format("Invalid fastest command, see help.\n"));
			return;
		}

		COpenStreetMap::TNodeID src;
		COpenStreetMap::TNodeID dest;
		try 
		{
			src = std::stol(command[1]);
			dest = std::stol(command[2]);
		} 
		catch(std::invalid_argument) 
		{
			errsink->Write(Format("Invalid fastest parameter, see help.\n"));
			return;
		}
		
		path.clear();
		pathtime = planner->FindFastestPath(src, dest, path);

		if (pathtime == CPathRouter::NoPathExists)
			errsink->Write(Format("Unable to find fastest path between " + std::to_string(src) + " to " + std::to_string(dest) + ".\n"));
		
		else 
			outsink->Write(Format("Fastest path takes" + Time(pathtime) + ".\n"));
	} 
	
	void Shortest(std::vector<std::string> command)
	{
		if (command.size() < 3) {
			errsink->Write(Format("Invalid shortest command, see help.\n"));
			return;
		}
		
		COpenStreetMap::TNodeID src;
		COpenStreetMap::TNodeID dest;
		try
		{
			src = std::stol(command[1]);
			dest = std::stol(command[2]);
		} 
		catch(std::invalid_argument) {
			errsink->Write(Format("Invalid shortest parameter, see help.\n"));
			return;
		}

		std::vector < COpenStreetMap::TNodeID > p;
		auto distance = planner->FindShortestPath(src, dest, p);

		if (distance == CPathRouter::NoPathExists)
			errsink->Write(Format("Unable to find shortest path between " + command[1] + " to " + command[2] + ".\n"));

		else {
			// https://stackoverflow.com/questions/29200635/convert-float-to-string-with-precision-number-of-decimal-digits-specified
			std::stringstream stream;
			stream << std::fixed << std::setprecision(1) << distance;
			std::string dist = stream.str();

			outsink->Write(Format("Shortest path is " + dist + " mi.\n"));
		}
	}

	void Save() {
		if (path.size() == 0) 
		{
			errsink->Write(Format("No valid path to save, see help.\n"));
			return;
		}

		int last = path.size() - 1;
		std::string file = std::to_string(path[0].second) + "_" + std::to_string(path[last].second) + "_" + std::to_string(pathtime) + "hr.csv";
		auto savesink = results->CreateSink(file);
		savesink->Write(Format("mode,node_id\n"));

		// Writing all steps
		std::string trans, output;
		for (int i = 0; i < path.size(); i++) {
			if (path[i].first == CTransportationPlanner::ETransportationMode::Bus)
				trans = "Bus";
			else if (path[i].first == CTransportationPlanner::ETransportationMode::Bike)
				trans = "Bike";
			else if (path[i].first == CTransportationPlanner::ETransportationMode::Walk)
				trans = "Walk";
			
			output += trans + "," + std::to_string(path[i].second);
			if (i != last)
				output += "\n";
		}

		savesink->Write(Format(output));
		outsink->Write(Format("Path saved to <results>/" + file + "\n"));
	}

	void Print()
	{
		if (path.size() == 0) 
			errsink->Write(Format("No valid path to print, see help.\n"));
		
		else
		{
			std::vector <std::string> desc;
			planner->GetPathDescription(path, desc);
			
			for (int i = 0; i < desc.size(); i++) 
				outsink->Write(Format(desc[i] + "\n"));
		}
	}

	std::vector<char> Format(std::string s)
	{
		return std::vector<char>(s.begin(), s.end());
	}

	std::string Time(double hours) 
	{
		int hr = std::floor(hours);
		double remaining = (hours - hr) * 60;
		int min = std::floor(remaining);
		int sec = std::round((remaining - min) * 60);

		std::string time;
		if (hr)
			time += " " + std::to_string(hr) + " hr";
		
		if (min)
			time += " " + std::to_string(min) + " min";
		
		if (sec)
			time += " " + std::to_string(sec) + " sec";

		return time;
	}

	std::string Coordinate(double c, bool lat = true)
	{
		std::string direction;
		double coord = c;
		if (coord < 0) {
			if (lat)
				direction = "S";
			else
				direction = "W";
			coord *= -1;
		}
		else 
		{
			if (lat)
				direction = "N";
			else
				direction = "E";
		}
			
		int degrees = std::floor(coord);
		double remaining = (coord - degrees) * 60;
		int minutes = std::floor(remaining);
		int seconds = std::round((remaining - minutes) * 60);

		return std::to_string(degrees) + "d " + std::to_string(minutes) + "' " + std::to_string(seconds) + "\" " + direction;
	}
};

CTransportationPlannerCommandLine::CTransportationPlannerCommandLine(std::shared_ptr<CDataSource> cmdsrc, std::shared_ptr<CDataSink> outsink, std::shared_ptr<CDataSink> errsink, std::shared_ptr<CDataFactory> results, std::shared_ptr<CTransportationPlanner> planner) {
	DImplementation = std::make_unique<SImplementation>(cmdsrc, outsink, errsink, results, planner);
}

CTransportationPlannerCommandLine::~CTransportationPlannerCommandLine() {}

bool CTransportationPlannerCommandLine::ProcessCommands() {
	return DImplementation->ProcessCommands();
}
