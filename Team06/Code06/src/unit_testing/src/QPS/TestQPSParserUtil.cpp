#include "QPS/parser/ParserUtil.h"
#include "catch.hpp"
using namespace std;

TEST_CASE("FindSuchThat / such that present  / original wordlist not modified "
          "returns index of such") {
    vector<string> wordList = {"random", "string", "/asd", "such", "that", "asd", ",ksl"};
    vector<string> duplicateWordList = wordList;
    vector<int> expected{3};
    vector<int> result = findSuchThat(wordList);

    CHECK(wordList == duplicateWordList);
    REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / multiple such that present / returns index of all "
          "such that") {
    vector<string> wordList = {"such", "such", "that", "such", "that", "that", ",such"};
    vector<int> expected{1, 3};
    vector<int> result = findSuchThat(wordList);

    REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / only such, no that present / return empty vector") {
    vector<string> wordList = {"such", "such", "asdf", "such", "qwe", "123", ",asd"};
    vector<int> expected;
    vector<int> result = findSuchThat(wordList);

    REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / only that, no such present / return empty vector") {
    vector<string> wordList = {"that", "that", "asdf", "that", "qwe", "123", ",asd"};
    vector<int> expected;
    vector<int> result = findSuchThat(wordList);

    REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / both such that present, but not consecutive in that "
          "order / return empty vector") {
    vector<string> wordList = {"that", "such", "asdf", "that", "such", "123,", "that"};
    vector<int> expected;
    vector<int> result = findSuchThat(wordList);

    REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / neither such nor that present / return empty vector") {
    vector<string> wordList = {"qwee", "asd", "asdf", "vxx", "qwe", "123", ",asd"};
    vector<int> expected;
    vector<int> result = findSuchThat(wordList);

    REQUIRE(expected == result);
}

TEST_CASE("FindWith / one with present followed by fixed string / return correct index") {
    vector<string> wordList = { "qwee", "asd", "with", "\"asd\"", "qwe", "123", ",asd" };
    vector<int> expected = { 2 };
    vector<int> result = findWith(wordList);

    REQUIRE(expected == result);
}

TEST_CASE("FindWith / one with present followed by ident / return correct index") {
    vector<string> wordList = { "qwee", "with", "stuff", "\"asd\"", "qwe", "123", ",asd" };
    vector<int> expected = { 1 };
    vector<int> result = findWith(wordList);

    REQUIRE(expected == result);
}

TEST_CASE("FindWith / one with present followed by integer / return correct index") {
    vector<string> wordList = { "qwee", "with", "1", "\"asd\"", "qwe", "123", ",asd" };
    vector<int> expected = { 1 };
    vector<int> result = findWith(wordList);

    REQUIRE(expected == result);
}

TEST_CASE("FindWith / one with present followed by with, similar to ident case / return index of first with") {
    vector<string> wordList = { "qwee", "with", "with", ".qs", "=", "123", ",asd" };
    vector<int> expected = { 1 };
    vector<int> result = findWith(wordList);

    REQUIRE(expected == result);
}

TEST_CASE("FindWith / one with present followed by bracket / return empty vector") {
    vector<string> wordList = { "qwee", "with", "(", "\"asd\")", "qwe", "123", ",asd" };
    vector<int> expected = {};
    vector<int> result = findWith(wordList);

    REQUIRE(expected == result);
}

TEST_CASE("FindWith / multiple with present with correct subsequent char / return all correct indexes") {
    vector<string> wordList = { "qwee", "with", "\"", "asd\")", "with", "123", ",asd" };
    vector<int> expected = {1, 4};
    vector<int> result = findWith(wordList);

    REQUIRE(expected == result);
}

TEST_CASE("stringToWordList / string with consecutive spaces / return wordlist "
          "without whitespace") {
    string input = " a  b c   d		e \n \n f ";
    vector<string> expected = {"a", "b", "c", "d", "e", "f"};
    vector<string> output = stringToWordList(input);
    REQUIRE(expected == output);
}

TEST_CASE("extractParameters / one clause / return expected") {
    string input = "a(sd,\"x*y\")";
    tuple<string, vector<string>> expectedTuple("a", {"sd", "\"x*y\""});

    tuple<string, vector<string>> output = extractParameters(input, "(", ")", ",");
    REQUIRE(expectedTuple == output);
}

TEST_CASE("extractParameters / one clause with brackets in third parameter / "
          "return expected") {
    string input = "a(sd,\"(x*y)+(z*y)\")";
    tuple<string, vector<string>> expectedTuple("a", {"sd", "\"(x*y)+(z*y)\""});
    tuple<string, vector<string>> output = extractParameters(input, "(", ")", ",");
    REQUIRE(expectedTuple == output);
}

TEST_CASE("extractParameters / pattern string with wild cards / return expected") {
    string input = "a(sd,_\"(x*y)+(z*y)\"_)";
    tuple<string, vector<string>> expectedTuple("a", {"sd", "_\"(x*y)+(z*y)\"_"});
    tuple<string, vector<string>> output = extractParameters(input, "(", ")", ",");
    REQUIRE(expectedTuple == output);
}

TEST_CASE("extractParameters / one clause / return expected ") {
    string input = "a(sd\"x*y\")";
    tuple<string, vector<string>> expectedTuple("a", {"sd\"x*y\""});
    tuple<string, vector<string>> output = extractParameters(input, "(", ")", ",");
    REQUIRE(expectedTuple == output);
}

TEST_CASE("extractParameters / no outerParam, different braces/ return expected") {
    string input = "<asd,\"x*y\">";
    tuple<string, vector<string>> expectedTuple("", {"asd", "\"x*y\""});
    tuple<string, vector<string>> output = extractParameters(input, "<", ">", ",");
    REQUIRE(expectedTuple == output);
}

TEST_CASE("extractSubStringUntilDelimiter / empty string, non-empty delimiter, "
          "0 <= start < string length / throws error") {
    string inputString = "";
    string inputDelimiter = ",";
    int start = 0;
    REQUIRE_THROWS(extractSubStringUntilDelimiter(inputString, start, inputDelimiter));
}

TEST_CASE("extractSubStringUntilDelimiter / non-empty string, empty delimiter, "
          "0 <= start < string length / throws error") {
    string inputString = "helloMyName is johnny";
    string inputDelimiter = "";
    int start = 0;
    REQUIRE_THROWS(extractSubStringUntilDelimiter(inputString, start, inputDelimiter));
}

TEST_CASE("extractSubStringUntilDelimiter / non-empty string, non-empty "
          "delimiter, start >= string length / throws error") {
    string inputString = "helloMyName is johnny";
    string inputDelimiter = ",";
    int start = inputString.size();
    REQUIRE_THROWS(extractSubStringUntilDelimiter(inputString, start, inputDelimiter));
}

TEST_CASE("extractSubStringUntilDelimiter / non-empty string, non-empty "
          "delimiter, start < 0 / throws error") {
    string inputString = "helloMyName is johnny";
    string inputDelimiter = ",";
    int start = -1;
    REQUIRE_THROWS(extractSubStringUntilDelimiter(inputString, start, inputDelimiter));
}

TEST_CASE("extractSubStringUntilDelimiter / non-empty string, non-empty "
          "delimiter not present, start = 0 / return tuple with full string "
          "and end pos at string length") {
    string inputString = "helloMyName is johnny";
    string inputDelimiter = ",";
    int start = 0;

    tuple<string, size_t, bool> expected{inputString, inputString.size(), false};
    tuple<string, size_t, bool> output = extractSubStringUntilDelimiter(inputString, start, inputDelimiter);
    CHECK(expected == output);
}

TEST_CASE("extractSubStringUntilDelimiter / non-empty string, non-empty "
          "delimiter present, start = 0 / return tuple with expected substring and next starting position") {
    string inputStringFront = "helloMyName,";
    string inputStringBack = " is johnny ";
    string inputString = inputStringFront + inputStringBack;
    string expectedString = "helloMyName";
    string inputDelimiter = ",";
    int start = 0;

    tuple<string, size_t, bool> expected{expectedString, inputStringFront.size(), true};
    tuple<string, size_t, bool> output = extractSubStringUntilDelimiter(inputString, start, inputDelimiter);
    CHECK(expected == output);
}

TEST_CASE("extractSubStringUntilDelimiter / non-empty string, non-empty "
          "delimiter present multiple times, start = 0 / return tuple with expected substring "
          "and next starting position") {
    string inputStringFront = "helloMyName,";
    string inputStringBack = " is, johnny ";
    string inputString = inputStringFront + inputStringBack;
    string expectedString = "helloMyName";
    string inputDelimiter = ",";
    int start = 0;

    tuple<string, size_t, bool> expected{expectedString, inputStringFront.size(), true};
    tuple<string, size_t, bool> output = extractSubStringUntilDelimiter(inputString, start, inputDelimiter);
    CHECK(expected == output);
}

TEST_CASE("extractSubStringUntilDelimiter / non-empty string, non-empty "
          "delimiter present multiple times, start after first delimiter / return tuple with "
          "expected substring "
          "and next starting position") {
    string inputStringFront = "helloMyName,";
    string inputStringMiddle = " is,";
    string inputStringBack = " johnny ";
    string inputString = inputStringFront + inputStringMiddle + inputStringBack;
    string expectedString = " is";
    string inputDelimiter = ",";
    int start = inputStringFront.size();

    tuple<string, size_t, bool> expected{expectedString, inputStringFront.size() + inputStringMiddle.size(), true};
    tuple<string, size_t, bool> output = extractSubStringUntilDelimiter(inputString, start, inputDelimiter);
    CHECK(expected == output);
}