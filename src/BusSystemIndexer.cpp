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

struct CBusSystemIndexer::SImplementation
{
    // https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key  
    struct pair_hash
    {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2> &p) const
        {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ h2;
        }
    };

    std::vector<TNodeID> stopIDlist;
    std::unordered_map<TNodeID, CBusSystem::TStopID> stops;
    std::vector<std::string> routeNamelist;
    std::shared_ptr<CBusSystem> bus;
    std::unordered_map<std::pair<TNodeID, TNodeID>, std::unordered_set< std::shared_ptr<SRoute> >, pair_hash > table;

    SImplementation(std::shared_ptr<CBusSystem> bussystem)
    {
        bus = bussystem;

        for (std::size_t i = 0; i < bussystem->RouteCount(); i++)
        {
            auto currRoute = bussystem->RouteByIndex(i);
            routeNamelist.push_back(currRoute->Name());

            for (std::size_t j = 0; j < bussystem->RouteByIndex(i)->StopCount() - 1; j++)
            {
                for (std::size_t k = j + 1; k < bussystem->RouteByIndex(i)->StopCount(); k++)
                {
                    
                    auto src = bussystem->StopByID(currRoute->GetStopID(j))->NodeID();
                    auto dest = bussystem->StopByID(currRoute->GetStopID(k))->NodeID();
                    
                    auto key = std::make_pair(src, dest);
                    
                    if (table.find(key) != table.end())
                    {
                        table[key].insert(currRoute);
                    }
                    else
                    {
                        table[key] = {currRoute};
                    }
                }
            }
        }

        for (std::size_t i = 0; i < bussystem->StopCount(); i++)
        {
            stopIDlist.push_back(bussystem->StopByIndex(i)->NodeID());
            stops[bussystem->StopByIndex(i)->NodeID()] = bussystem->StopByIndex(i)->ID();
        }
        std::sort(stopIDlist.begin(), stopIDlist.end());
        std::sort(routeNamelist.begin(), routeNamelist.end());
    }

    std::size_t StopCount() const noexcept
    {
        return stopIDlist.size();
    }
    std::size_t RouteCount() const noexcept
    {
        return routeNamelist.size();
    }
    std::shared_ptr<SStop> SortedStopByIndex(std::size_t index) const noexcept
    {
        
        try
        {
            return bus->StopByID(stops.at(stopIDlist.at(index)));
        }
        catch (const std::out_of_range &excpt)
        {
            return nullptr;
        }
    }
    std::shared_ptr<SRoute> SortedRouteByIndex(std::size_t index) const noexcept
    {
        try
        {
            return bus->RouteByName(routeNamelist.at(index));
        }
        catch (const std::out_of_range &excpt)
        {
            return nullptr;
        }
    }
    std::shared_ptr<SStop> StopByNodeID(TNodeID id) const noexcept
    {
        return bus->StopByID(stops.at(id));
    }
    bool RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute>> &routes) const noexcept
    {
        auto key = std::make_pair(src, dest);
        auto find = table.find(key);
        
        if (find != table.end()) 
        {
            routes = find->second;
            return true;
        }

        return false;
    }
    bool RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept
    {
        auto key = std::make_pair(src, dest);
        return (table.find(key) != table.end());
    }
};

CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem) {
	DImplementation = std::make_unique<SImplementation>(bussystem);
}

CBusSystemIndexer::~CBusSystemIndexer() {}

std::size_t CBusSystemIndexer::StopCount() const noexcept {
	return DImplementation->StopCount();
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept {
	return DImplementation->RouteCount();
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept {
	return DImplementation->SortedStopByIndex(index);
}

std::shared_ptr<CBusSystem::SRoute> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept {
	return DImplementation->SortedRouteByIndex(index);
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::StopByNodeID(CStreetMap::TNodeID id) const noexcept {
	return DImplementation->StopByNodeID(id);
}

bool CBusSystemIndexer::RoutesByNodeIDs(CStreetMap::TNodeID src, CStreetMap::TNodeID dest, std::unordered_set<std::shared_ptr<CBusSystem::SRoute> > &routes) const noexcept {
	return DImplementation->RoutesByNodeIDs(src, dest, routes);
}

bool CBusSystemIndexer::RouteBetweenNodeIDs(CStreetMap::TNodeID src, CStreetMap::TNodeID dest) const noexcept {
	return DImplementation->RouteBetweenNodeIDs(src, dest);
}
