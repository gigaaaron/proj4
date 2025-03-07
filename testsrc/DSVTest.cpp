#include <gtest/gtest.h>
#include "DSVReader.h"
#include "DSVWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(DSVReader, ReadRow)
{
    std::vector<std::string> read;
    std::vector<std::string> results;

    auto source = std::make_shared<CStringDataSource>("goodnight,stars,\"goodnight,air\"\ngoodnight");
    auto reader = std::make_unique<CDSVReader>(source, ',');
    
    reader->ReadRow(read);
    EXPECT_FALSE(reader->End());
    results={"goodnight","stars","goodnight,air"};
    EXPECT_EQ(read,results);
    
    reader->ReadRow(read);
    EXPECT_TRUE(reader->End());
    results={"goodnight"};
    EXPECT_EQ(read,results);
}

TEST(DSVWriter, WriteRow)
{
    auto sampleSink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sampleSink, ',');

    std::vector<std::string> input = {"", "pot", "of greed"};
    writer.WriteRow(input);
    EXPECT_EQ(sampleSink->String(), ",pot,of greed");
}

//"blue" eyes, white dragon
TEST(DSVWriter, quoteallTrue){
    auto sampleSink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sampleSink, ',', true);

    std::vector<std::string>row={"blue", "eyes, white","dragon"};
    writer.WriteRow(row);

    EXPECT_EQ(sampleSink->String(), "\"blue\",\"eyes, white\",\"dragon\"");
}

