
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
		// try shit
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
		// Find vertex ID
		CPathRouter::TVertexID srcVertex = std::find(nodes.begin(), nodes.end(), map->NodeByID(src)) - nodes.begin();
		CPathRouter::TVertexID destVertex = std::find(nodes.begin(), nodes.end(), map->NodeByID(dest)) - nodes.begin();

		if (srcVertex == nodes.size() || destVertex == nodes.size())
			return CPathRouter::NoPathExists;
		
		// Calculate bike and bus/walk time to compare
		std::vector<TNodeID> bikepath, buswalkpath;
		double biketime = fastestBike->FindShortestPath(srcVertex, destVertex, bikepath);
		double buswalktime = fastestBusWalk->FindShortestPath(srcVertex, destVertex, buswalkpath);

		if (biketime < buswalktime)
		{
			for (int i = 0; i < bikepath.size(); i++) 
				path.push_back({ETransportationMode::Bike, nodes[bikepath[i]]});
		
			return biketime;
		} 
		else
		{
			for (int i = 1; i < buswalkpath.size(); i++) {
				TNodeID src = nodes[buswalkpath[i - 1]];
				TNodeID dest = nodes[buswalkpath[i]];
				
				auto walktime = Weight(src, dest, true, walkspeed);
				if (bus->RouteBetweenNodeIDs(src, dest) && stoptime < walktime)
					path.push_back({ETransportationMode::Bus, nodes[buswalkpath[i]]});
				else
					path.push_back({ETransportationMode::Walk, nodes[buswalkpath[i]]});
			}
			return buswalktime;
		}
		return CPathRouter::NoPathExists;
	}
};
