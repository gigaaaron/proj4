#include <gtest/gtest.h>
#include "DSVReader.h"
#include "StringDataSource.h"
#include "StringDataSink.h"
#include "CSVBusSystem.h"

TEST(CSVBusSystem, StopCount)
{
    auto stop_source = std::make_shared<CStringDataSource>("stop_id,node_id\n22043,2849810514\n22358,2849805223\n22274,3954294407\n22230,5598639595");
    auto route_source = std::make_shared<CStringDataSource>("route,stop_id\nA,22258\nA,22169nB,22274\nB,22230");

    auto stop_reader = std::make_shared<CDSVReader>(stop_source, ',');
    auto route_reader = std::make_shared<CDSVReader>(route_source, ',');

    auto bus = std::make_shared<CCSVBusSystem>(stop_reader, route_reader);
    

    EXPECT_EQ(bus->StopCount(), 4);
}

TEST(CSVBusSystem, RouteCount)
{
    auto stop_source = std::make_shared<CStringDataSource>("stop_id,node_id\n22043,2849810514\n22358,2849805223\n22274,3954294407\n22230,5598639595");
    auto route_source = std::make_shared<CStringDataSource>("route,stop_id\nA,22258\nA,22169nB,22274\nB,22230");
    
    auto stop_reader = std::make_shared<CDSVReader>(stop_source, ',');
    auto route_reader = std::make_shared<CDSVReader>(route_source, ',');

    auto bus = std::make_shared<CCSVBusSystem>(stop_reader, route_reader);

    EXPECT_EQ(bus->RouteCount(), 2);
}

TEST(CSVBusSystem, StopByIndex)
{
    auto stop_source = std::make_shared<CStringDataSource>("stop_id,node_id\n22043,2849810514\n22358,2849805223\n22274,3954294407\n22230,5598639595");
    auto route_source = std::make_shared<CStringDataSource>("route,stop_id\nA,22258\nA,22169nB,22274\nB,22230");
    
    auto stop_reader = std::make_shared<CDSVReader>(stop_source, ',');
    auto route_reader = std::make_shared<CDSVReader>(route_source, ',');

    auto bus = std::make_shared<CCSVBusSystem>(stop_reader, route_reader);

    auto stop1 = bus->StopByIndex(0);
    EXPECT_EQ(stop1->ID(), 22043);

    auto stop2 = bus->StopByIndex(1);
    EXPECT_EQ(stop2->ID(), 22358);
}

TEST(CSVBusSystem, StopByID)
{
    auto stop_source = std::make_shared<CStringDataSource>("stop_id,node_id\n22043,2849810514\n22358,2849805223\n22274,3954294407\n22230,5598639595");
    auto route_source = std::make_shared<CStringDataSource>("route,stop_id\nA,22258\nA,22169nB,22274\nB,22230");
    
    auto stop_reader = std::make_shared<CDSVReader>(stop_source, ',');
    auto route_reader = std::make_shared<CDSVReader>(route_source, ',');

    auto bus = std::make_shared<CCSVBusSystem>(stop_reader, route_reader);

    auto stop1 = bus->StopByIndex(0);
    EXPECT_EQ(stop1->ID(), 22043);

    auto stop2 = bus->StopByIndex(1);
    EXPECT_EQ(stop2->ID(), 22358);
}

TEST(CSVBusSystem, RouteByIndex)
{
    auto stop_source = std::make_shared<CStringDataSource>("stop_id,node_id\n22043,2849810514\n22358,2849805223\n22274,3954294407\n22230,5598639595");
    auto route_source = std::make_shared<CStringDataSource>("route,stop_id\nA,22258\nA,22169nB,22274\nB,22230");
    
    auto stop_reader = std::make_shared<CDSVReader>(stop_source, ',');
    auto route_reader = std::make_shared<CDSVReader>(route_source, ',');

    auto bus = std::make_shared<CCSVBusSystem>(stop_reader, route_reader);

    auto route1 = bus->RouteByIndex(0);
    EXPECT_EQ(route1->Name(), "A");

    auto route2 = bus->RouteByIndex(1);
    EXPECT_EQ(route2->Name(), "B");
}

TEST(CSVBusSystem, RouteByName)
{
    auto stop_source = std::make_shared<CStringDataSource>("stop_id,node_id\n22043,2849810514\n22358,2849805223\n22274,3954294407\n22230,5598639595");
    auto route_source = std::make_shared<CStringDataSource>("route,stop_id\nA,22258\nA,22169nB,22274\nB,22230"); 
    
    auto stop_reader = std::make_shared<CDSVReader>(stop_source, ',');
    auto route_reader = std::make_shared<CDSVReader>(route_source, ',');

    auto bus = std::make_shared<CCSVBusSystem>(stop_reader, route_reader);

    auto route = bus->RouteByName("A");
    EXPECT_EQ(route->Name(), "A");
}
