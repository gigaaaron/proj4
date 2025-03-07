#include "CSVBusSystem.h"
#include "BusSystemIndexer.h"
#include <vector>
#include <stdexcept>

struct CBusSystemIndexer::SImplementation
{

    std::vector<TNodeID> stopIDlist;
    std::vector<std::string> routeNamelist;
    std::shared_ptr<CBusSystem> bus;

    CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem)
    {
        bus = bussystem;
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
    }
    bool RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept;
};

