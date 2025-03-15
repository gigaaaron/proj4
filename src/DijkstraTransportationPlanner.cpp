
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

	std::shared_ptr<CStreetMap> map;
	std::shared_ptr<CBusSystemIndexer> bus;
	double walkspeed, bikespeed, speedlimit, stoptime;

	std::vector<CStreetMap::TNodeID> nodes;
	std::vector<CStreetMap::TNodeID> sortednodes;
	std::unordered_map<std::pair<TNodeID, TNodeID>, std::string, pair_hash> edgenames;
	std::shared_ptr<CDijkstraPathRouter> shortest;
	std::shared_ptr<CDijkstraPathRouter> fastestBike;
	std::shared_ptr<CDijkstraPathRouter> fastestBusWalk;

	SImplementation(std::shared_ptr<SConfiguration> config) 
	{
		map = config->StreetMap();
		bus = std::make_shared<CBusSystemIndexer>(config->BusSystem());
		walkspeed = config->WalkSpeed();
		bikespeed = config->BikeSpeed();
		speedlimit = config->DefaultSpeedLimit();
		stoptime = config->BusStopTime() / 3600;
	
		// Add nodes
		for (int i = 0; i < map->NodeCount(); i++) 
		{
			TNodeID id = map->NodeByIndex(i)->ID();
			nodes.push_back(id);

			shortest->AddVertex(id);
			fastestBike->AddVertex(id);
			fastestBusWalk->AddVertex(id);
		}

		sortednodes = nodes;
		std::sort(sortednodes.begin(), sortednodes.end());

		// Add edges
		for (int i = 0; i < map->WayCount(); i++) 
		{
			auto way = map->WayByIndex(i);

			// Process each pair of nodes in the way
			for (int j = 0; way->NodeCount() - 1; j++)
			{
				TNodeID src = way->GetNodeID(j);
				TNodeID dest = way->GetNodeID(j + 1);

				CPathRouter::TVertexID srcVertex = std::find(nodes.begin(), nodes.end(), map->NodeByID(src)) - nodes.begin();
				CPathRouter::TVertexID destVertex = std::find(nodes.begin(), nodes.end(), map->NodeByID(dest)) - nodes.begin();
				
				if (srcVertex == nodes.size() || destVertex == nodes.size())
					continue;

				// Add edge names
				std::pair<TNodeID, TNodeID> pair1(src, dest);
				std::pair<TNodeID, TNodeID> pair2(dest, src);
				edgenames[pair1] = way->GetAttribute("name");
				edgenames[pair2] = way->GetAttribute("name");

				bool bidir = (way->GetAttribute("oneway") == "yes");
				
				// Populate Dijkstra graphs
				
				// Distance
				shortest->AddEdge(srcVertex, destVertex, Weight(src, dest), bidir);

				// Biking
				if (way->GetAttribute("bicycle") != "no")
					fastestBike->AddEdge(srcVertex, destVertex, Weight(src, dest, true, bikespeed), bidir);
				
				// Bus/Walking
				fastestBusWalk->AddEdge(srcVertex, destVertex, Weight(src, dest, true, walkspeed), true);
				if (bus->RouteBetweenNodeIDs(src, dest))
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

	double Weight(TNodeID Src, TNodeID Dest, bool fastest = false, double speed = 1, bool bus = false) {
		auto src = map->NodeByID(Src)->Location();
		auto dest = map->NodeByID(Dest)->Location();

		double w = SGeographicUtils::HaversineDistanceInMiles(src, dest);
		if (fastest)
		{
			w /= speed;

			if (bus)
				w += stoptime;
		}

		return w;
	}

	std::size_t NodeCount() const noexcept {
		return map->NodeCount();
	}

	std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept
	{
		try
            {
                return map->NodeByID(sortednodes.at(index));
            }
            catch (std::out_of_range &excpt)
            {
                return nullptr;
            }
	}

	double FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID> &path) 
	{
		CPathRouter::TVertexID srcVertex = std::find(nodes.begin(), nodes.end(), map->NodeByID(src)) - nodes.begin();
		CPathRouter::TVertexID destVertex = std::find(nodes.begin(), nodes.end(), map->NodeByID(dest)) - nodes.begin();

		if (srcVertex == nodes.size() || destVertex == nodes.size())
			return CPathRouter::NoPathExists;

		double distance = shortest->FindShortestPath(srcVertex, destVertex, path);
		for (int i = 0; i < path.size(); i++)
			path[i] = nodes[path[i]];
		
		return distance;
	}

	double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path)
	{
		CPathRouter::TVertexID srcVertexID = std::find(sortedNodeIDs.begin(), sortedNodeIDs.end(), src) - sortedNodeIDs.begin();
		CPathRouter::TVertexID destVertexID = std::find(sortedNodeIDs.begin(), sortedNodeIDs.end(), dest) - sortedNodeIDs.begin();SIZE_MAX;


		if (srcVertexID == sortedNodeIDs.size() || destVertexID == sortedNodeIDs.size())
			return CPathRouter::NoPathExists;
		
		std::vector<TNodeID> bikePath;
		std::vector<TNodeID> busWalkPath;
		double bikeTime = fastestBike->FindShortestPath(srcVertexID, destVertexID, bikePath);
		double busWalkTime = fastestBusWalk->FindShortestPath(srcVertexID, destVertexID, busWalkPath);

		
		if (bikeTime < busWalkTime) {
			for (size_t i = 0; i < bikePath.size(); i++)
				path.push_back({ETransportationMode::Bike, VertexToNodes[bikePath[i]]});
			
			return bikeTime;
		}
		
		else if (busWalkTime < bikeTime) {
			path.push_back({ETransportationMode::Walk, VertexToNodes[busWalkPath[0]]});
			
			for (size_t i = 1; i < busWalkPath.size(); i++) {
				TNodeID prev = VertexToNodes[busWalkPath[i-1]];
				TNodeID curr = VertexToNodes[busWalkPath[i]];

				// Use bus for this edge if a bus route exists
				if (busIndexer->RouteBetweenNodeIDs(prev, curr))
					path.push_back({ETransportationMode::Bus, VertexToNodes[busWalkPath[i]]});
				else
					path.push_back({ETransportationMode::Walk, VertexToNodes[busWalkPath[i]]});
			}
			return busWalkTime;
		}

		return CPathRouter::NoPathExists;
	}

	bool GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const {

		auto startcoords = map->NodeByID(path[0].second)->Location();
		desc.push_back("Start at " + Coordinate(startcoords.first) + ", " + Coordinate(startcoords.second, false));

		TNodeID previous, current;

		for (int i = 1; i < path.size(); i++) 
		{
			previous = path[i - 1].second;
			current = path[i].second;

			if (path[i].first == ETransportationMode::Bus)
			{
				// Find contiguous steps where bus is taken
				std::vector<TNodeID> stops;
				for (int j = 0; i + j < path.size(); j++)
				{
					if (path[i + j].first != ETransportationMode::Bus)
						break;
					stops.push_back(path[i + j].second);
				}

				std::pair< std::string, TNodeID > busroute = LongestBusRoute(stops);

				auto srcstop = std::to_string(bus->StopByNodeID(previous)->ID());
				auto deststop = std::to_string(bus->StopByNodeID(path[busroute.second].second)->ID());

				desc.push_back("Take Bus " + busroute.first + " from stop " + srcstop + " to stop " + deststop);

				for (int j = i; j < path.size(); j++)
				{
					if (path[j].second == busroute.second)
					{
						i = j;
						break;
					}
				}
			}

			else 
			{	
				// Street name
				std::string street;
				auto search = edgenames.find(std::make_pair(previous, current));
				if (search != edgenames.end())
				{
					street = "along " + search->second;
				
					auto search2 = edgenames.find(std::make_pair(current, path[i + 1].second));
					while (search2 != edgenames.end() && search->second == search2->second) {
						if (i >= path.size())
							break;
						
						i++;
						current = path[i].second;
						search2 = edgenames.find(std::make_pair(current, path[i + 1].second));
					}
				} 
				
				else 
				{
					search = edgenames.find(std::make_pair(current, path[i + 1].second));

					for (i; i < path.size(); i++)
					{
						if (search != edgenames.end())
						{
							street = " toward " + search->second;
							break;
						}

						else if (i == path.size() - 1) 
						{
							street = " toward End";
							break;
						}

						current = path[i].second;
						search = edgenames.find(std::make_pair(current, path[i + 1].second));
					}
				}

				auto previouscoords = map->NodeByID(previous)->Location();
				auto currentcoords = map->NodeByID(current)->Location();

				// https://stackoverflow.com/questions/29200635/convert-float-to-string-with-precision-number-of-decimal-digits-specified
				double distance = SGeographicUtils::HaversineDistanceInMiles(previouscoords, currentcoords);
				std::stringstream stream;
				stream << std::fixed << std::setprecision(1) << distance;
				
				std::string dist = stream.str();
				std::string direction = SGeographicUtils::BearingToDirection(SGeographicUtils::CalculateBearing(previouscoords, currentcoords));

				if (path[i].first == CTransportationPlanner::ETransportationMode::Bike)
					desc.push_back("Bike " + direction + street + " for " + dist);
				else if (path[i].first == CTransportationPlanner::ETransportationMode::Walk)
					desc.push_back("Walk " + direction + street + " for " + dist);
			}
		}

		auto currentcoords = map->NodeByID(current)->Location();
		desc.push_back("End at " + Coordinate(currentcoords.first) + ", " + Coordinate(currentcoords.second, false));
		return true;
	}

	std::pair< std::string, TNodeID> LongestBusRoute(std::vector < TNodeID > stops) const
	{
		int i = stops.size() - 1;
		while (!bus->RouteBetweenNodeIDs(stops[0], stops[i]))
			i--;
		
		std::unordered_set<std::shared_ptr<CBusSystem::SRoute>> routes;
		bus->RoutesByNodeIDs(stops[0], stops[i], routes);
		
		// Sort available routes and return first
		std::vector<std::string> routenames;
		for (auto route : routes)
			routenames.push_back(route->Name());
		
		std::sort(routenames.begin(), routenames.end());

		return std::make_pair(routenames[0], stops[i]);
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
