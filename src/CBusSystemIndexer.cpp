#include "CSVBusSystem.h"
#include "BusSystem.h"
#include "BusSystemIndexer.h"
#include <vector>
#include <stdexcept>
#include <unordered_map>

struct CBusSystemIndexer::SImplementation
{

    std::vector<TNodeID> stopIDlist;
    std::vector<std::string> routeNamelist;
    std::shared_ptr<CBusSystem> bus;
    std::unordered_map<std::pair<TNodeID, TNodeID>, std::vector<std::string>> table;

    CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem)
    {
        bus = bussystem;
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

        for (int i = 0; i < routes.size(); i++)
        {
            for (int j = 0; j < routes[i].size() - 1; j++)
            {
                for (int k = j + 1; k < routes[i].size(); k++)
                {
                    auto key = std::make_pair(routes[i][j], routes[i][k]);
                    if (table.find(key) != table.end())
                    {
                        table[key].push_back(routes[i]);
                    }
                    else
                    {
                        a = std::vector<string>;
                        a.push_back(routes[i]);
                        table[key, a];
                    }
                }
            }
        }

        for (int i = 0; i < bussystem->stops.size(); i++)
        {
            stopIDlist.push_back(bussystem->stops[i]->nodeid);
        }
        std::sort(stopIDlist.begin(), stopIDlist.end());

        for (int i = 0; i < bussystem->routes.size(); i++)
        {
            routeNamelist.push_back(bussystem->routes[i]->name);
        }
        std::sort(routeNamelist.begin(), routeNamelist.end());
    }

    ~CBusSystemIndexer();

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
            return bus->StopByID(stopIDlist.at(index));
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
        return bus->StopByID(id);
    }
    bool RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute>> &routes) const noexcept
    {
        return table[src, dest];
    }
    bool RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept
    {
        return table[src, dest];
    }
};
