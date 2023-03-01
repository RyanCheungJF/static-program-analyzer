#include "catch.hpp"
#include <iostream>
#include "qps/parser/ParserUtil.h"
using namespace std;


TEST_CASE("FindSuchThat / such that present  / original wordlist not modified returns index of such") {
	vector<string> wordList = { "random", "string", "/asd", "such", "that", "asd", ",ksl" };
	vector<string> duplicateWordList = wordList;
	vector<int> expected{ 3 };
	vector<int> result = findSuchThat(wordList);

	CHECK(wordList == duplicateWordList);
	REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / multiple such that present / returns index of all such that") {
	vector<string> wordList = { "such", "such", "that", "such", "that", "that", ",such" };
	vector<int> expected{ 1, 3 };
	vector<int> result = findSuchThat(wordList);
	
	REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / only such, no that present / return -1") {
	vector<string> wordList = { "such", "such", "asdf", "such", "qwe", "123", ",asd" };
	vector<int> expected;
	vector<int> result = findSuchThat(wordList);

	REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / only that, no such present / return -1") {
	vector<string> wordList = { "that", "that", "asdf", "that", "qwe", "123", ",asd" };
	vector<int> expected;
	vector<int> result = findSuchThat(wordList);

	REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / both such that present, but not consecutive in that order / return -1") {
	vector<string> wordList = { "that", "such", "asdf", "that", "such", "123,", "that" };
	vector<int> expected;
	vector<int> result = findSuchThat(wordList);

	REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / neither such nor that present / return -1") {
	vector<string> wordList = { "qwee", "asd", "asdf", "vxx", "qwe", "123", ",asd" };
	vector<int> expected;
	vector<int> result = findSuchThat(wordList);

	REQUIRE(expected == result);
}

TEST_CASE("stringToWordList / string with consecutive spaces / return wordlist without whitespace") {
	string input = " a  b c   d		e \n \n f ";
	vector<string> expected = { "a", "b", "c", "d", "e", "f" };
	vector<string> output = stringToWordList(input);
	REQUIRE(expected == output);
}

TEST_CASE("extractParameters / one clause / return vector with one tuple") {
	string input = "a(sd,\"x*y\")";
	tuple<string, string, string> expectedTuple("a", "sd", "\"x*y\"");

	tuple<string, string, string> output = extractParameters(input);
	REQUIRE(expectedTuple == output);
}

TEST_CASE("extractParameters / one clause with brackets in third parameter / return vector with one tuple") {
	string input = "a(sd,\"(x*y)+(z*y)\")";
	tuple<string, string, string> expectedTuple("a", "sd", "\"(x*y)+(z*y)\"");
	tuple<string, string, string> output = extractParameters(input);
	REQUIRE(expectedTuple == output);
}

TEST_CASE("extractParameters / pattern string with wild cards / return vector with one tuple") {
	string input = "a(sd,_\"(x*y)+(z*y)\"_)";
	tuple<string, string, string> expectedTuple("a", "sd", "_\"(x*y)+(z*y)\"_");
	tuple<string, string, string> output = extractParameters(input);
	REQUIRE(expectedTuple == output);
}

TEST_CASE("extractParameters / one clause with no closing bracket / throws error") {
	string input = "a(sd,\"x*y\"";
	REQUIRE_THROWS(extractParameters(input));
}

TEST_CASE("extractParameters / one clause with no comma / throws error") {
	string input = "a(sd\"x*y\")";
	REQUIRE_THROWS(extractParameters(input));
}

TEST_CASE("extractParameters / one clause with no opening bracket / throws error") {
	string input = "asd,\"x*y\")";
	REQUIRE_THROWS(extractParameters(input));
}

TEST_CASE("extractParameters / one clause with not proper format / throws error") {
	string input = "asd,\"x*y\"";
	REQUIRE_THROWS(extractParameters(input));
}

TEST_CASE("extractSubStringUntilDelimiter / empty string, non-empty delimiter, 0 <= start < string length / throws error") {
	string inputString = "";
	string inputDelimiter = ",";
	int start = 0;
	REQUIRE_THROWS(extractSubStringUntilDelimiter(inputString, start, inputDelimiter));
}

TEST_CASE("extractSubStringUntilDelimiter / non-empty string, empty delimiter, 0 <= start < string length / throws error") {
	string inputString = "helloMyName is johnny";
	string inputDelimiter = "";
	int start = 0;
    REQUIRE_THROWS(extractSubStringUntilDelimiter(inputString, start, inputDelimiter));
}

TEST_CASE("extractSubStringUntilDelimiter / non-empty string, non-empty delimiter, start >= string length / throws error") {
	string inputString = "helloMyName is johnny";
	string inputDelimiter = ",";
	int start = inputString.size();
    REQUIRE_THROWS(extractSubStringUntilDelimiter(inputString, start, inputDelimiter));
}

TEST_CASE("extractSubStringUntilDelimiter / non-empty string, non-empty delimiter, start < 0 / throws error") {
	string inputString = "helloMyName is johnny";
	string inputDelimiter = ",";
	int start = -1;
    REQUIRE_THROWS(extractSubStringUntilDelimiter(inputString, start, inputDelimiter));
}

TEST_CASE("extractSubStringUntilDelimiter / non-empty string, non-empty delimiter not present, start = 0 / return tuple with full string and end pos at string length") {
	string inputString = "helloMyName is johnny";
	string inputDelimiter = ",";
	int start = 0;

	tuple<string, size_t> expected{ inputString, inputString.size() };
	tuple<string, size_t> output = extractSubStringUntilDelimiter(inputString, start, inputDelimiter);
	CHECK(expected == output);
}