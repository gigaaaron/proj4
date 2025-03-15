#include "DijkstraTransportationPlanner.h"
#include "GeographicUtils.h"
#include "DijkstraPathRouter.h"
#include "BusSystemIndexer.h"
#include "CSVBusSystem.h"
#include "XMLReader.h"
#include "DSVReader.h"
#include "StringDataSource.h"
#include "StringUtils.h"
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <set>
#include <sstream>
#include <iomanip>
#include <cmath>

struct CDijkstraTransportationPlanner::SImplementation {
	// https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key
	struct pair_hash 
	{
		template <class T1, class T2>
		std::size_t operator () (const std::pair<T1,T2> &p) const {
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);

			// Mainly for demonstration purposes, i.e. works but is overly simple
			// In the real world, use sth. like boost.hash_combine
			return h1 ^ h2;  
		}
	};

	std::shared_ptr<CStreetMap> streetMap;
	std::shared_ptr<CBusSystemIndexer> busIndexer;

	std::vector<TNodeID> nodes;
	std::vector<TNodeID> sortednodes;
	std::unordered_map<std::pair<TNodeID, TNodeID>, std::string, pair_hash> edgenames;

	std::shared_ptr<CDijkstraPathRouter> shortestPath;
	std::shared_ptr<CDijkstraPathRouter> fastestBike;
	std::shared_ptr<CDijkstraPathRouter> fastestBusWalk;

	double walkspeed, bikespeed, speedlimit, stoptime;

	SImplementation(std::shared_ptr<SConfiguration> config)
	{
		streetMap = config->StreetMap();
		busIndexer = std::make_shared<CBusSystemIndexer>(config->BusSystem());
		walkspeed = config->WalkSpeed();
		bikespeed = config->BikeSpeed();
		speedlimit = config->DefaultSpeedLimit();
		stoptime = config->BusStopTime() / 3600;

		shortestPath = std::make_shared<CDijkstraPathRouter>();
		fastestBike = std::make_shared<CDijkstraPathRouter>();
		fastestBusWalk = std::make_shared<CDijkstraPathRouter>();
		
		// Add nodes
		for (int i = 0; i < streetMap->NodeCount(); i++)
		{
			TNodeID node = streetMap->NodeByIndex(i)->ID();
			nodes.push_back(node);
			sortednodes.push_back(node);

			shortestPath->AddVertex(node);
			fastestBike->AddVertex(node);
			fastestBusWalk->AddVertex(node);
		}
		std::sort(sortednodes.begin(), sortednodes.end());

		for (int i = 0; i < streetMap->WayCount(); i++)
		{
			auto way = streetMap->WayByIndex(i);
			bool bidir = (way->GetAttribute("oneway") != "yes");

			for (int j = 1; j < way->NodeCount(); j++)
			{
				TNodeID src = way->GetNodeID(j - 1);
				TNodeID dest = way->GetNodeID(j);

				CPathRouter::TVertexID srcVertex = std::find(nodes.begin(), nodes.end(), src) - nodes.begin();
				CPathRouter::TVertexID destVertex = std::find(nodes.begin(), nodes.end(), dest) - nodes.begin();
				
				if (srcVertex == nodes.size() || destVertex == nodes.size())
					continue;

				// Edge names
				edgenames[std::make_pair(src, dest)] = way->GetAttribute("name");
				edgenames[std::make_pair(dest, src)] = way->GetAttribute("name");

				// Populate path routers
				shortestPath->AddEdge(srcVertex, destVertex, Weight(src, dest), bidir);

				if (way->GetAttribute("bicycle") != "no")
					fastestBike->AddEdge(srcVertex, destVertex, Weight(src, dest, true, bikespeed), bidir);
				
				fastestBusWalk->AddEdge(srcVertex, destVertex, Weight(src, dest, true, walkspeed), true);
				if (busIndexer->RouteBetweenNodeIDs(src, dest)) 
				{
					double speed;
					if (way->HasAttribute("maxspeed"))
						speed = std::stod(StringUtils::Replace(way->GetAttribute("maxspeed"), " mph", ""));
					else
						speed = speedlimit;
					
					fastestBusWalk->AddEdge(srcVertex, destVertex, Weight(src, dest, true, speed, true));
				}
			}
		}
	}

	double Weight(TNodeID src, TNodeID dest, bool fastest = false, double speed = 1, bool bus = false)
	{
		auto srccoords = streetMap->NodeByID(src)->Location();
		auto destcoords = streetMap->NodeByID(dest)->Location();

		double weight = SGeographicUtils::HaversineDistanceInMiles(srccoords, destcoords);

		if (fastest)
		{
			weight /= speed;

			if (bus)
				weight += stoptime;
		}

		return weight;
	}

	std::size_t NodeCount() const noexcept { return streetMap->NodeCount(); }

	std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept
	{
		try 
		{
			return streetMap->NodeByID(sortednodes.at(index));
		} 
		catch (std::out_of_range &excpt) 
		{
			return nullptr;
		}
	}

	double FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID> &path)
	{
		CPathRouter::TVertexID srcVertex = std::find(nodes.begin(), nodes.end(), src) - nodes.begin();
		CPathRouter::TVertexID destVertex = std::find(nodes.begin(), nodes.end(), dest) - nodes.begin();
		
		if (srcVertex == nodes.size() || destVertex == nodes.size())
			return CPathRouter::NoPathExists;
		
		double distance = shortestPath->FindShortestPath(srcVertex, destVertex, path);
		
		// Convert vertex IDs to node IDs
		for (int i = 0; i < path.size(); i++)
			path[i] = nodes[path[i]];
		
		return distance;
	}

	double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path)
	{
		CPathRouter::TVertexID srcVertex = std::find(nodes.begin(), nodes.end(), src) - nodes.begin();
		CPathRouter::TVertexID destVertex = std::find(nodes.begin(), nodes.end(),dest) - nodes.begin();
		
		if (srcVertex == nodes.size() || destVertex == nodes.size())
			return CPathRouter::NoPathExists;
		
		std::vector<TNodeID> bikepath, buswalkpath;
		double biketime = fastestBike->FindShortestPath(srcVertex, destVertex, bikepath);
		double buswalktime = fastestBusWalk->FindShortestPath(srcVertex, destVertex, buswalkpath);

		if (buswalktime > biketime)
		{
			for (int i = 0; i < bikepath.size(); i++)
				path.push_back({ETransportationMode::Bike, nodes[bikepath[i]]});
		
			return biketime;
		}
		else
		{
			path.push_back({ETransportationMode::Walk, nodes[bikepath[0]]});
			
			for (int i = 1; i < buswalkpath.size(); i++)
			{
				TNodeID src = nodes[buswalkpath[i-1]];
				TNodeID dest = nodes[buswalkpath[i]];

				if (busIndexer->RouteBetweenNodeIDs(src, dest))
					path.push_back({ETransportationMode::Bus, nodes[buswalkpath[i]]});
				else
				path.push_back({ETransportationMode::Walk, nodes[buswalkpath[i]]});
			}
			return buswalktime;
		}

		return CPathRouter::NoPathExists;
	}

	bool GetPathDescription(const std::vector < TTripStep > &path, std::vector<std::string> &desc) const
	{
		auto startlocation = streetMap->NodeByID(path[0].second)->Location();
		desc.push_back("Start at " + Coordinate(startlocation.first) + ", " + Coordinate(startlocation.second, false));

		TNodeID src, dest;
		double distance = 0;

		for (int i = 1; i < path.size(); i++)
		{
			src = path[i-1].second;
			dest = path[i].second;

			if (path[i].first == CTransportationPlanner::ETransportationMode::Bus)
			{
				// Create vector of consecutive bus rides
				std::vector<TNodeID> stops;
				stops.push_back(src);
				for (int j = 0; j + i < path.size(); j++)
				{
					if (path[i + j].first != CTransportationPlanner::ETransportationMode::Bus)
						break;
					
					stops.push_back(path[i + j].second);
				}

				std::pair<std::string, TNodeID> busroute = LongestBusRoute(stops);

				while (path[i].second != busroute.second)
					i++;
				
				std::string srcstop = std::to_string(busIndexer->StopByNodeID(src)->ID());
				std::string deststop = std::to_string(busIndexer->StopByNodeID(dest)->ID());
				
				desc.push_back("Take Bus " + busroute.first + " from stop " + srcstop + " to stop " + deststop);
			}
			else
			{
				auto edgename = edgenames.find(std::make_pair(src, dest));
				std::string street;
				if (edgename != edgenames.end() && edgename->second != "") 
				{
					street = " along " + edgename->second;

					auto edgecont = edgenames.find(std::make_pair(dest, path[i+1].second));
					while (edgecont != edgenames.end() && edgecont->second == edgename->second)
					{
						if (i == path.size())
							break;
						i++;
						auto edgecont = edgenames.find(std::make_pair(dest, path[i+1].second));
					}
				} 
				else // No street name
				{
					edgename = edgenames.find(std::make_pair(dest, path[i+1].second));
					while (edgename == edgenames.end())
					{
						if (i == path.size() - 1)
						{
							street = " toward End";
							break;
						}	
						
						auto srclocation = streetMap->NodeByID(src)->Location();
						auto destlocation = streetMap->NodeByID(dest)->Location();
						distance += SGeographicUtils::HaversineDistanceInMiles(srclocation, destlocation);
						
						i++;
						src = dest;
						dest = path[i].second;
						
						edgename = edgenames.find(std::make_pair(dest, path[i+1].second));
					}
					if (edgename != edgenames.end())
						street = " toward " + edgename->second;
				}

				auto srclocation = streetMap->NodeByID(src)->Location();
				auto destlocation = streetMap->NodeByID(dest)->Location();
				distance += SGeographicUtils::HaversineDistanceInMiles(srclocation, destlocation);

				// https://stackoverflow.com/questions/29200635/convert-float-to-string-with-precision-number-of-decimal-digits-specified
				std::stringstream stream;
				stream << std::fixed << std::setprecision(1) << distance;
				std::string dist = stream.str() + " mi";

				distance = 0;

				// Find direction
				std::string dir = SGeographicUtils::BearingToDirection(SGeographicUtils::CalculateBearing(srclocation, destlocation));

				if (path[i].first == CTransportationPlanner::ETransportationMode::Bike)
					desc.push_back("Bike " + dir + street + " for " + dist);
				else if (path[i].first == CTransportationPlanner::ETransportationMode::Walk)
					desc.push_back("Walk " + dir + street + " for " + dist);

			}
		}
		auto endlocation = streetMap->NodeByID(dest)->Location();

		desc.push_back("End at " + Coordinate(endlocation.first) + ", " + Coordinate(endlocation.second, false));
		return true;
	}

	std::string Coordinate(double c, bool lat = true) const
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

	std::pair< std::string, TNodeID> LongestBusRoute(std::vector < TNodeID > stops) const
	{
		int i = stops.size() - 1;
		while (!busIndexer->RouteBetweenNodeIDs(stops[0], stops[i]))
			i--;
		
		std::unordered_set<std::shared_ptr<CBusSystem::SRoute>> routes;
		busIndexer->RoutesByNodeIDs(stops[0], stops[i], routes);
		
		// Sort available routes and return first
		std::vector<std::string> routenames;
		for (auto route : routes)
			routenames.push_back(route->Name());
		
		std::sort(routenames.begin(), routenames.end());

		return std::make_pair(routenames[0], stops[i]);
	}

};

CDijkstraTransportationPlanner::CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config) {
	DImplementation = std::make_unique<SImplementation>(config);
}

CDijkstraTransportationPlanner::~CDijkstraTransportationPlanner() {}

std::size_t CDijkstraTransportationPlanner::NodeCount() const noexcept {
	return DImplementation->NodeCount();
}

std::shared_ptr<CStreetMap::SNode> CDijkstraTransportationPlanner::SortedNodeByIndex(std::size_t index) const noexcept {
	return DImplementation->SortedNodeByIndex(index);
}

double CDijkstraTransportationPlanner::FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path) {
	return DImplementation->FindShortestPath(src, dest, path);
}

double CDijkstraTransportationPlanner::FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path) {
	return DImplementation->FindFastestPath(src, dest, path);
}

bool CDijkstraTransportationPlanner::GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const {
	return DImplementation->GetPathDescription(path, desc);
}
