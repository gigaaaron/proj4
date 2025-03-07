#include "DSVWriter.h"
#include "StringUtils.h"
#include <iostream>

struct CDSVWriter::SImplementation
{
    char separator;
    std::shared_ptr<CDataSink> stnk;
    bool quote;
    bool written = false;

    SImplementation(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
    {
        stnk = sink;
        separator = delimiter;
        quote = quoteall;
    }

    bool WriteRow(const std::vector<std::string> &row)
    {
        std::vector<std::string> temp;

        if (written)
            temp.push_back("\n");

        for (int i = 0; i < row.size(); i++)
        {
            temp.push_back(StringUtils::Replace(row[i], "\"\"", "\""));
            if (quote)
            {
                temp[i] = '"' + temp[i] + '"';
            }
            else if (temp[i].find("\n") != std::string::npos || temp[i].find(separator) != std::string::npos || temp[i].find("\"") != std::string::npos)
            {
                temp[i] = '"' + temp[i] + '"';
            }
        }

        std::vector<char> tempchar;

        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[i].length(); j++)
                tempchar.push_back(temp[i][j]);

            if (i != temp.size() - 1)
                tempchar.push_back(separator);
        }
        stnk->Write(tempchar);
        written = true;
        return true;
    }
};

CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> src, char delimiter, bool quoteall)
{
    DImplementation = std::make_unique<SImplementation>(src, delimiter, quoteall);
}
CDSVWriter::~CDSVWriter() {}
bool CDSVWriter::WriteRow(const std::vector<std::string> &row)
{
    return DImplementation->WriteRow(row);
}
