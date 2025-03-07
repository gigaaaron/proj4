#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
	std::string test;

	test = "Hello World";
	EXPECT_EQ(StringUtils::Slice(test, 1), "ello World");
	EXPECT_EQ(StringUtils::Slice(test, 2, 4), "ll");
	EXPECT_EQ(StringUtils::Slice(test, -2, -1), "l");
	EXPECT_EQ(StringUtils::Slice(test, -100, 0), "Hello World");
	EXPECT_EQ(StringUtils::Slice(test, 0, -100), "");
    
}

TEST(StringUtilsTest, Capitalize){
    std::string test;

	test = "Hello World";
	EXPECT_EQ(StringUtils::Capitalize(test), "Hello world");

	test = "hello woRld";
	EXPECT_EQ(StringUtils::Capitalize(test), "Hello world");

	test = "12345!!!!abc";
	EXPECT_EQ(StringUtils::Capitalize(test), "12345!!!!abc");

}

TEST(StringUtilsTest, Upper){
    std::string test;

	test = "Hello World";
	EXPECT_EQ(StringUtils::Upper(test), "HELLO WORLD");

	test = "hello woRld";
	EXPECT_EQ(StringUtils::Upper(test), "HELLO WORLD");

	test = "12345!!!!abc";
	EXPECT_EQ(StringUtils::Upper(test), "12345!!!!ABC");
}

TEST(StringUtilsTest, Lower){
    std::string test;

	test = "Hello World";
	EXPECT_EQ(StringUtils::Lower(test), "hello world");

	test = "hello woRld";
	EXPECT_EQ(StringUtils::Lower(test), "hello world");

	test = "12345!!!!ABc";
	EXPECT_EQ(StringUtils::Lower(test), "12345!!!!abc");
}

TEST(StringUtilsTest, LStrip){
	std::string test;

	test = "   Hello World  ";
	EXPECT_EQ(StringUtils::LStrip(test), "Hello World  ");

	test = "Hello World  ";
	EXPECT_EQ(StringUtils::LStrip(test), "Hello World  ");

	test = "   ";
	EXPECT_EQ(StringUtils::LStrip(test), "");
    
}

TEST(StringUtilsTest, RStrip){
	std::string test;

	test = "   Hello World  ";
	EXPECT_EQ(StringUtils::RStrip(test), "   Hello World");

	test = "  Hello World";
	EXPECT_EQ(StringUtils::RStrip(test), "  Hello World");

	test = "   ";
	EXPECT_EQ(StringUtils::RStrip(test), "");
    
}

TEST(StringUtilsTest, Strip){
    std::string test;

	test = "   Hello World  ";
	EXPECT_EQ(StringUtils::Strip(test), "Hello World");

	test = " Hello   World";
	EXPECT_EQ(StringUtils::Strip(test), "Hello   World");

	test = "   ";
	EXPECT_EQ(StringUtils::Strip(test), "");
}

TEST(StringUtilsTest, Center){
    std::string test; 
	
	test = "abc";
	EXPECT_EQ(StringUtils::Center(test, 0, '*'), "abc");
	EXPECT_EQ(StringUtils::Center(test, 7), "  abc  ");
	EXPECT_EQ(StringUtils::Center(test, 7, '-'), "--abc--");

	test = "12";
	EXPECT_EQ(StringUtils::Center(test, 5, '*'), "**12*");

	test = "123";
	EXPECT_EQ(StringUtils::Center(test, 6, '*'), "*123**");
}

TEST(StringUtilsTest, LJust){
	std::string test;

	test = "abc";
	EXPECT_EQ(StringUtils::LJust(test, 0, '*'), "abc");
	EXPECT_EQ(StringUtils::LJust(test, 7), "abc    ");
	EXPECT_EQ(StringUtils::LJust(test, 7, '-'), "abc----");    
}

TEST(StringUtilsTest, RJust){
    std::string test;

	test = "abc";
	EXPECT_EQ(StringUtils::RJust(test, 2, '*'), "abc");
	EXPECT_EQ(StringUtils::RJust(test, 7), "    abc");
	EXPECT_EQ(StringUtils::RJust(test, 7, '-'), "----abc");  
}

TEST(StringUtilsTest, Replace){
    std::string test;

	test = "abc abc abc123 Abc abcabc";
	EXPECT_EQ(StringUtils::Replace(test, "abc", "123"), "123 123 123123 Abc 123123");
	EXPECT_EQ(StringUtils::Replace(test, "ABC", "123"), "abc abc abc123 Abc abcabc");

	test = "abc";
	EXPECT_EQ(StringUtils::Replace(test, "", " "), " a b c ");
	EXPECT_EQ(StringUtils::Replace(test, "ab", ""), "c");
}

TEST(StringUtilsTest, Split){
	std::string test = "Hello World";
	std::vector< std::string > result;

	result = {"Hello", "World"};
	EXPECT_EQ(StringUtils::Split(test), result);

	result = {"", " World"};
	EXPECT_EQ(StringUtils::Split(test, "Hello"), result);

	result = {"Hello World"};
	EXPECT_EQ(StringUtils::Split(test, "*"), result);
    
}

TEST(StringUtilsTest, Join){
	std::vector< std::string > test;

	test = {"Hello", "World"};
    EXPECT_EQ(StringUtils::Join(" ", test), "Hello World");
	EXPECT_EQ(StringUtils::Join("", test), "HelloWorld");
	EXPECT_EQ(StringUtils::Join("---", test), "Hello---World");

	test = {};
	EXPECT_EQ(StringUtils::Join(" ", test), "");
}

TEST(StringUtilsTest, ExpandTabs){
	std::string test;

	test = "123\t1234";
	EXPECT_EQ(StringUtils::ExpandTabs(test), "123 1234");
	EXPECT_EQ(StringUtils::ExpandTabs(test, 3), "123   1234");

	test = "1234\t1234";
	EXPECT_EQ(StringUtils::ExpandTabs(test), "1234    1234");

	test = "12\n34\t1234";
	EXPECT_EQ(StringUtils::ExpandTabs(test), "12\n34  1234");
    
}

TEST(StringUtilsTest, EditDistance){
    std::string test1;
	std::string test2;

	test1 = "sitting";
	test2 = "kitten";
	EXPECT_EQ(StringUtils::EditDistance(test1, test2), 3);

	test1 = "uniformed";
	test2 = "uninformed";
	EXPECT_EQ(StringUtils::EditDistance(test1, test2), 1);

	test1 = "AGHHHHHH I DON'T WANT TO WRITE TEST CASES ANYMORE";
	test2 = "aghhhhhh i don't want to write test cases anymore";
	EXPECT_EQ(StringUtils::EditDistance(test1, test2), 40);
	EXPECT_EQ(StringUtils::EditDistance(test1, test2, true), 0);
}