#include "StringUtils.h"

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
    // Return substring between start (inclusive) and end (exclusive, 0 = end of string)

    int length = str.length();

    // Check and convert indices
    if (start < 0) {
        start += length;

        // If start is still negative (abs. value greater than length)
        if (start < 0)
            start = 0;
    }

    if (end < 0)
        end += length;
    else if (end == 0 || end > length)
        end = length;

    // Return empty string when:
    // start >= end | start >= length | (end + length) <= 0
    if (start >= end || start >= length || end <= 0)
        return "";
    
    return str.substr(start, end - start);
}

std::string Capitalize(const std::string &str) noexcept{
    // Capitalizes first letter in string and lowercases all others

    std::string capString;
    int length = str.length();

    capString += toupper(str[0]);
    for (int i = 1; i < length; i++) 
        capString += tolower(str[i]);

    return capString;
}

std::string Upper(const std::string &str) noexcept{
    // Capitalizes all letters in string

    std::string upperString;
    int length = str.length();

    for (int i = 0; i < length; i++)
        upperString += toupper(str[i]);

    return upperString;
}

std::string Lower(const std::string &str) noexcept{
    // Lowercases all letters in string

    std::string lowerString;
    int length = str.length();

    for (int i = 0; i < length; i++)
        lowerString += tolower(str[i]);

    return lowerString;
}

std::string LStrip(const std::string &str) noexcept{
    // Removes whitespace characters from left

    // Iterate through whitespace at start of string and break at non-whitespace
    int i = 0;
    while (str[i] == ' ')
        i++; 

    return Slice(str, i, 0);
}

std::string RStrip(const std::string &str) noexcept{
    // Removes whitespace characters from right

    int length = str.length();

    // Iterate through whitespace at end of string and break at non-whitespace
    int i = length - 1;
    while (str[i] == ' ')
        i--;

    return str.substr(0, i + 1);
}

std::string Strip(const std::string &str) noexcept{
    // Removes whitespace characters from both left and right
    return LStrip(RStrip(str));
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    // Centers string in between fill characters
    
    std::string centerString;
    int length = str.length();
    width -= length;

    if (width < 0)
        return str;
    
    // Overflow char for uneven fill depends on length of original string
    // See: https://stackoverflow.com/questions/73699051/how-to-implement-string-center-by-padding-the-extra-character-always-on-the-le 
    int left = width / 2 + (width & width + length & 1);
    int right = width - left;

    centerString.append(left, fill);
    centerString += str;
    centerString.append(right, fill);

    return centerString;
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    // Adds (width - length) fill characters to right side of string

    std::string lJustString;
    int length = str.length();
    width -= length;

    if (width < 0)
        return str;

    lJustString = str;
    lJustString.append(width, fill);
    
    return lJustString;
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    // Adds (width - length) fill characters to left side of string

    std::string rJustString;
    int length = str.length();
    width -= length;

    if (width < 0)
        return str;

    rJustString.append(width, fill);
    rJustString += str;
    
    return rJustString;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
    // Replaces all occurrences of old with rep

    std::string repString;
    int length = str.length();

    // If old == "", insert rep between every character
    // If str is empty, insert rep once
    if (old == "") {
        repString += rep;
        for (int i = 0; i < length; i++) {
            repString += str[i];
            repString += rep;
        }
        return repString;
    }

    // Split string at every instance of old and stitch back together with rep
    std::vector< std::string > strSplit = Split(str, old);
    for (int i = 0; i < strSplit.size(); i++) {
        repString += strSplit[i];

        // Add rep after every substring except last one
        if (i != strSplit.size() - 1)
            repString += rep;
    } 

    return repString;
}

std::vector< std::string > Split(const std::string &str, const std::string &splt) noexcept{
    // Splits string into substrings before and after each occurrence of splt

    std::vector < std::string > strSplit;
    // Modifiable copy of str
    std::string strCopy = str;

    // If splt == "", treat as whitespace
    std::string split = splt;
    if (splt == "")
        split = " ";
    
    // Iterate through to find each instance of splt
    int end = strCopy.find(split);
    while (end != std::string::npos) {
        // Add substring from start to beginning to vector
        strSplit.push_back(strCopy.substr(0, end));
        // Remove split substring from rest of the string
        strCopy = Slice(strCopy, end + split.length(), 0);
        // Find next instance of splt 
        end = strCopy.find(split);
    }
    // Add last substring
    strSplit.push_back(strCopy);

    return strSplit;
}

std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    // Joins substrings into one string, separated by str
    
    if (vect.empty())
        return "";

    std::string joinString;
    int lastString = vect.size() - 1;

    for (int i = 0; i < lastString; i++)
        joinString += vect[i] + str;
    joinString += vect[lastString];

    return joinString;
}

std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    // Replaces tab characters with appropriate number of spaces to reach next tabspace

    std::string expandedString;
    std::vector< std::string > strings = Split(str, "\t");
    int charCount = 0;
    int spaceCount;

    for (int i = 0; i < strings.size() - 1; i++) {
        // Count spaces in substring
        for (int j = 0; j < strings[i].length(); j++) {
            // Reset space count for newlines
            if (strings[i][j] != '\n' && strings[i][j] != '\r')
                charCount++;
            else
                charCount = 0;
        }
        expandedString += strings[i];
        
        // Calculate actual number of spaces to add to string
        spaceCount = tabsize - (charCount % tabsize);
        if (!spaceCount)
            spaceCount = tabsize;

        expandedString.append(spaceCount, ' ');
        charCount = 0;
    }
    // Add last substring
    expandedString += strings[strings.size() - 1];

    return expandedString;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
    // Calculates edit distance between two strings

    // Modifiable copies of both strings
    std::string leftString = left;
    std::string rightString = right;

    if (ignorecase) {
        leftString = Lower(left);
        rightString = Lower(right);
    }

    // Levenshtein edit distance:
    // If current chars match, take diagonal
    // If current chars do not match, take min(diagonal, top, left) + 1

    int y = leftString.length() + 1;
    int x = rightString.length() + 1;
    int matrix[y][x];

    // Initialize first column
    for (int i = 0; i < y; i++) 
        matrix[i][0] = i;
    // Initialize first row
    for (int i = 1; i < x; i++)
        matrix[0][i] = i;

    // Populate matrix
    for (int i = 1; i < y; i++) {
        for (int j = 1; j < x; j++) {
            // If match
            if (leftString[i - 1] == rightString[j - 1])
                matrix[i][j] = matrix[i - 1][j - 1];

            // If not match
            else
                matrix[i][j] = std::min({matrix[i - 1][j - 1], matrix[i - 1][j], matrix[i][j - 1]}) + 1;
        }
    }

    return matrix[y - 1][x - 1];
}

};