#include "DSVReader.h"
#include "StringUtils.h"

struct CDSVReader::SImplementation
{
    std::shared_ptr<CDataSource> source;
    char separator;

    SImplementation(std::shared_ptr<CDataSource> src, char delimiter)
    {
        source = src;
        separator = delimiter;

        if (separator == '"')
            separator = ',';
    }


    // Returns true if all rows have been read from the DSV
    bool End() const
    {
        return source->End();
    }

    // Returns true if the row is successfully read, one string will be put in
    // the row per column
    bool ReadRow(std::vector<std::string> &row)
    {
        row.clear();
        // Find end of the row: non-quoted \n
        // Split row into strings based on non-quoted separator
        // An empty line is a valid row where there are no values.
        if (End())
            return false;

        int quotecount = 0;
        std::string currRow = "";

        while (!End())
        {
            char c;
            source->Get(c);

            if (c == '\n' && quotecount % 2 == 0)
            {
                break;
            }

            if (c == '"')
            {
                quotecount++;
 
            }

            currRow += c;
        }

        quotecount = 0;
        std::string temp = "";
        for (int i = 0; i < currRow.length(); i++)
        {

            if (currRow[i] == separator && quotecount % 2 == 0)
            {
                row.push_back(temp);
                temp = "";
            }
            else
            {
                if (currRow[i] == '"') {
                    quotecount++;
                }
                temp += currRow[i];
            }
        }
        row.push_back(temp);

        // Strip quotes
        for (int i = 0; i < row.size(); i++) {
            if (row[i][0] == '"' && row[i][row[i].size() - 1] == '"')
                row[i] = row[i].substr(1, row[i].length() - 2);
            
            row[i] = StringUtils::Replace(row[i], "\"\"", "\"");
        }

        return true;
    }
};

CDSVReader::CDSVReader(std::shared_ptr< CDataSource > src, char delimiter){
    DImplementation = std::make_unique<SImplementation>(src, delimiter);
}
CDSVReader::~CDSVReader(){}
bool CDSVReader::End() const{
    return DImplementation->End();
}
bool CDSVReader::ReadRow(std::vector<std::string> &row){
    return DImplementation->ReadRow(row);
}
