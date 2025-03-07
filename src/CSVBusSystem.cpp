#include "CSVBusSystem.h"
#include <stdexcept>
#include <iostream>

struct CCSVBusSystem::SImplementation
{
    struct SStop : public CBusSystem::SStop
    {
        TStopID stopid;
        CStreetMap::TNodeID nodeid;
        SStop(TStopID stupid, CStreetMap::TNodeID noodid)
        {
            stopid = stupid;
            nodeid = noodid;
        }
        ~SStop() {};
        TStopID ID() const noexcept { return stopid; }
        CStreetMap::TNodeID NodeID() const noexcept { return nodeid; }
    };

    struct SRoute : public CBusSystem::SRoute
    {
        std::string name;
        std::vector<TStopID> stops;

        SRoute(std::string route_name)
        {
            name = route_name;
        }

        ~SRoute() {};

        std::string Name() const noexcept { return name; }

        std::size_t StopCount() const noexcept {return stops.size();} 
        
        TStopID GetStopID(std::size_t index) const noexcept
        {
            try
            {
                return stops.at(index);
            }
            catch (std::out_of_range &excpt)
            {
                return InvalidStopID;
            }
        }
    };

    std::vector<std::shared_ptr<SStop>> stops;
    std::vector<std::shared_ptr<SRoute>> routes;

    SImplementation(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc)
    {
        long stopid;
        long nodeid;
        std::string routeid; //name
        std::vector<std::string> row;

        while (stopsrc->ReadRow(row))
        {
            try
            {
                stopid = std::stol(row[0]);
                nodeid = std::stol(row[1]);
            }
            catch (std::invalid_argument)
            {
                continue;
            }
            
            stops.push_back(std::make_shared<SStop>(stopid, nodeid));
        }

        while (routesrc->ReadRow(row))
        {
            try
            {
                routeid = row[0];
                stopid = std::stol(row[1]);
            }
            catch (std::invalid_argument)
            {
                continue;
            }

            int i = 0;
            for (i; i < routes.size(); i++)
            {
                if (routeid == routes[i]->Name())
                {
                    routes[i]->stops.push_back(stopid);
                    break;
                }
            }
            if (i == routes.size())
            {
                routes.push_back(std::make_shared<SRoute>(routeid));
                routes[i]->stops.push_back(stopid);
            }
        }
    }

    ~SImplementation() {};

    std::size_t StopCount() const { return stops.size(); }

    std::size_t RouteCount() const { return routes.size(); }

    std::shared_ptr<SStop> StopByIndex(std::size_t index) const
    {
        try
        {
            return stops.at(index);
        }
        catch (std::out_of_range &excpt)
        {
            return nullptr;
        }
        
    }

    std::shared_ptr<SStop> StopByID(TStopID id) const
    {
        for (int i = 0; i < StopCount(); i++)
        {
            if (id == stops[i]->stopid)
                return stops[i];
        }

        return nullptr;
    }

    std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const
    {
        try
        {
            return routes.at(index);
        }
        catch (std::out_of_range &excpt)
        {
            return nullptr;
        }

        
    }

    std::shared_ptr<SRoute> RouteByName(const std::string &name) const
    {
        for (int i = 0; i < RouteCount(); i++)
        {
            if (name == routes[i]->name)
                return routes[i];
        }

        return nullptr;
    }
};

CCSVBusSystem::CCSVBusSystem(std::shared_ptr< CDSVReader > stopsrc, std::shared_ptr< CDSVReader > routesrc){
	DImplementation = std::make_unique<SImplementation>(stopsrc, routesrc);
}


CCSVBusSystem::~CCSVBusSystem(){}

std::size_t CCSVBusSystem::StopCount() const noexcept{
    return DImplementation->StopCount();

}

std::size_t CCSVBusSystem::RouteCount() const noexcept{
    return DImplementation->RouteCount();

}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept{
    return DImplementation->StopByIndex(index);

}
std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(TStopID id) const noexcept{
    return DImplementation->StopByID(id);

}
std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept{
    return DImplementation->RouteByIndex(index);

}
std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string &name)const noexcept{
    return DImplementation->RouteByName(name);
}

