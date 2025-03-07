#include <gtest/gtest.h>
#include "OpenStreetMap.h"
#include "StreetMap.h"
#include "StringDataSource.h"
#include "XMLReader.h"
const std::string xml_data = R"(<?xml version='1.0' encoding='UTF-8'?>
    <osm version="0.6" generator="osmconvert 0.8.5">
        <node id="62208369" lat="38.5178523" lon="-121.7712408"/>
        <tag k="highway" v="motorway_junction"/>
        <node id="62209104" lat="38.535052" lon="-121.7408606"/>
        <node id="62224286" lat="38.5302841" lon="-121.7689756"/>
        <way id="8699536"> 
        <nd ref="258592863"/> 
        <nd ref="4399281377"/>
	</way>
    </osm>
    )";

TEST(OpenStreetMap, NodeCount)
{
    auto source = std::make_shared<CStringDataSource>(xml_data);
    auto reader = std::make_shared<CXMLReader>(source);
    COpenStreetMap osm(reader);

    EXPECT_EQ(osm.NodeCount(), 3);
}

TEST(OpenStreetMap, NodebyIndex)
{
    auto source = std::make_shared<CStringDataSource>(xml_data);
    auto reader = std::make_shared<CXMLReader>(source);
    COpenStreetMap osm(reader);

    auto node = osm.NodeByIndex(0);

    EXPECT_EQ(node->ID(), 62208369);
    EXPECT_EQ(node->Location().first, 38.5178523);
    EXPECT_EQ(node->Location().second, -121.7712408);
}

TEST(OpenStreetMap, NodebyID)
{
    auto source = std::make_shared<CStringDataSource>(xml_data);
    auto reader = std::make_shared<CXMLReader>(source);
    COpenStreetMap osm(reader);

    auto node = osm.NodeByID(62208369);

    EXPECT_EQ(node->ID(), 62208369);
    EXPECT_EQ(node->Location().first, 38.5178523);
    EXPECT_EQ(node->Location().second, -121.7712408);
}

TEST(OpenStreetMap, WayCount)
{
    auto source = std::make_shared<CStringDataSource>(xml_data);
    auto reader = std::make_shared<CXMLReader>(source);
    COpenStreetMap osm(reader);

    EXPECT_EQ(osm.WayCount(), 1);
}

TEST(OpenStreetMap, WayByIndex){
    auto source = std::make_shared<CStringDataSource>(xml_data);
    auto reader = std::make_shared<CXMLReader>(source);
    COpenStreetMap osm(reader);

    auto way = osm.WayByIndex(0);

    EXPECT_EQ(way->ID(), 8699536);  
    EXPECT_EQ(way->NodeCount(), 2);
}

TEST(OpenStreetMap, WayByID){
    auto source = std::make_shared<CStringDataSource>(xml_data);
    auto reader = std::make_shared<CXMLReader>(source);
    COpenStreetMap osm(reader);

    auto way = osm.WayByID(8699536); 

    EXPECT_EQ(way->ID(), 8699536);  
    EXPECT_EQ(way->NodeCount(), 2);
}

TEST(OpenStreetMap, Attributes) {
    auto source = std::make_shared<CStringDataSource>(xml_data);
    auto reader = std::make_shared<CXMLReader>(source);
    COpenStreetMap osm(reader);

    auto node = osm.NodeByID(62208369);

    EXPECT_TRUE(node->HasAttribute("highway"));  
    EXPECT_EQ(node->GetAttribute("highway"), "motorway_junction");  
}

TEST(OpenStreetMap, GetAttributeKey){
    auto source = std::make_shared<CStringDataSource>(xml_data);
    auto reader = std::make_shared<CXMLReader>(source);
    COpenStreetMap osm(reader);

    auto node = osm.NodeByID(62208369);

    EXPECT_EQ(node->GetAttributeKey(0), "highway"); 
}


