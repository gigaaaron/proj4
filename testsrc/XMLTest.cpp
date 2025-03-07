#include <gtest/gtest.h>
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(XMLReader, End) // check end()
{
    std::string sampleData = "<s1><s2>somedata</s2></s1>";
    auto source = std::make_shared<CStringDataSource>(sampleData);
    auto reader = std::make_unique<CXMLReader>(source);
    SXMLEntity entity;

    // hasnt read, should not be at end yet
    EXPECT_FALSE(reader->End());

    while (reader->ReadEntity(entity))
    {
    }
    EXPECT_TRUE(reader->End());
}

TEST(XMLReader, ReadEntityDType)
{
    std::string sampleData = "<s1><s2>somedata</s2></s1>";
    auto source = std::make_shared<CStringDataSource>(sampleData);
    auto reader = std::make_unique<CXMLReader>(source);
    SXMLEntity entity;

    bool result = reader->ReadEntity(entity);
    ASSERT_TRUE(result);
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);

    result = reader->ReadEntity(entity);
    ASSERT_TRUE(result);
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);

    result = reader->ReadEntity(entity);
    ASSERT_TRUE(result);
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);

    result = reader->ReadEntity(entity);
    ASSERT_TRUE(result);
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);

    result = reader->ReadEntity(entity);
    ASSERT_TRUE(result);
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);

    result = reader->ReadEntity(entity);
    ASSERT_FALSE(result);
}

TEST(XMLReader, Attributes)
{
    std::string sampleData = "<item name=\"card\"></item>";
    auto source = std::make_shared<CStringDataSource>(sampleData);
    auto reader = std::make_unique<CXMLReader>(source);
    SXMLEntity entity;

    bool result = reader->ReadEntity(entity);
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.AttributeValue("name"), "card");

    result = reader->ReadEntity(entity);
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
}

TEST(XMLReader, skipcdata)
{
    std::string sampleData = "<s1>somedata</s1>";
    auto source = std::make_shared<CStringDataSource>(sampleData);
    auto reader = std::make_unique<CXMLReader>(source);
    SXMLEntity entity;

    bool result = reader->ReadEntity(entity, true);
    ASSERT_TRUE(result);

    result = reader->ReadEntity(entity, true);
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    ASSERT_TRUE(result);
}

TEST(XMLWriter, WriteEntity){
    auto sampleSink = std::make_shared<CStringDataSink>();
    auto writer = std::make_unique<CXMLWriter>(sampleSink);
    
    SXMLEntity entity;
    entity.DNameData="s1";
    entity.DType=SXMLEntity::EType::StartElement;

    bool result = writer->WriteEntity(entity);
    EXPECT_TRUE(result);
    EXPECT_EQ(sampleSink->String(), "<s1>");

    SXMLEntity entity2;
    entity2.DNameData="s2";
    entity2.DType=SXMLEntity::EType::StartElement;

    result = writer->WriteEntity(entity2);
    EXPECT_TRUE(result);
    EXPECT_EQ(sampleSink->String(), "<s1>\n\t<s2>");

    SXMLEntity entity3;
    entity3.DNameData="somedata";
    entity3.DType=SXMLEntity::EType::CharData;

    result = writer->WriteEntity(entity3);
    EXPECT_TRUE(result);
    EXPECT_EQ(sampleSink->String(), "<s1>\n\t<s2>somedata");

    SXMLEntity entity4;
    entity4.DNameData="s2";
    entity4.DType=SXMLEntity::EType::EndElement;

    result = writer->WriteEntity(entity4);
    EXPECT_TRUE(result);
    EXPECT_EQ(sampleSink->String(), "<s1>\n\t<s2>somedata</s2>");

    SXMLEntity entity5;
    entity5.DNameData="s1";
    entity5.DType=SXMLEntity::EType::EndElement;

    result = writer->WriteEntity(entity5);
    EXPECT_TRUE(result);
    EXPECT_EQ(sampleSink->String(), "<s1>\n\t<s2>somedata</s2>\n</s1>");
}

TEST(XMLWriter, Flush)
{
    auto sampleSink = std::make_shared<CStringDataSink>();
    auto writer = std::make_unique<CXMLWriter>(sampleSink);
    
    SXMLEntity entity;
    entity.DNameData = "s1";
    entity.DType = SXMLEntity::EType::StartElement;
    writer->WriteEntity(entity);

    EXPECT_EQ(sampleSink->String(), "<s1>");
    bool result = writer->Flush();
    EXPECT_TRUE(result);  
    EXPECT_EQ(sampleSink->String(), "<s1></s1>");
}

TEST(XMLWriter, autograder) {
    std::string result = "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n\t<node id=\"62208369\" lat=\"38.5178523\" lon=\"-121.7712408\"/>\n</osm>";
    SXMLEntity entity1;
    entity1.DNameData = "osm";
    entity1.DAttributes.push_back({"version", "0.6"});
    entity1.DAttributes.push_back({"generator", "osmconvert 0.8.5"});
    entity1.DType = SXMLEntity::EType::StartElement;
    
    SXMLEntity entity2;
    entity2.DNameData = "node";
    entity2.DAttributes.push_back({"id", "62208369"});
    entity2.DAttributes.push_back({"lat", "38.5178523"});
    entity2.DAttributes.push_back({"lon", "-121.7712408"});
    entity2.DType = SXMLEntity::EType::CompleteElement;

    auto sampleSink = std::make_shared<CStringDataSink>();
    auto writer = std::make_unique<CXMLWriter>(sampleSink);

    writer->WriteEntity(entity1);
    writer->WriteEntity(entity2);
    writer->Flush();

    EXPECT_EQ(sampleSink->String(), result);
}
