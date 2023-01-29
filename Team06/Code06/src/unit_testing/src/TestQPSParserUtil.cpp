#include "catch.hpp"
#include "qps/parser/ParserUtil.h"
using namespace std;


TEST_CASE("FindSuchThat / such that present  / original wordlist not modified returns index of such") {
	vector<string> wordList = { "random", "string", "/asd", "such", "that", "asd", ",ksl" };
	vector<string> duplicateWordList = wordList;
	long expected = 3;
	long result = findSuchThat(wordList);

	CHECK(wordList == duplicateWordList);
	REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / multiple such that present / returns index of first such that") {
	vector<string> wordList = { "such", "such", "that", "such", "that", "that", ",such" };
	long expected = 1;
	long result = findSuchThat(wordList);
	
	REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / only such, no that present / return -1") {
	vector<string> wordList = { "such", "such", "asdf", "such", "qwe", "123", ",asd" };
	long expected = -1;
	long result = findSuchThat(wordList);

	REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / only that, no such present / return -1") {
	vector<string> wordList = { "that", "that", "asdf", "that", "qwe", "123", ",asd" };
	long expected = -1;
	long result = findSuchThat(wordList);

	REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / both such that present, but not consecutive in that order / return -1") {
	vector<string> wordList = { "that", "such", "asdf", "that", "such", "123,", "that" };
	long expected = -1;
	long result = findSuchThat(wordList);

	REQUIRE(expected == result);
}

TEST_CASE("FindSuchThat / neither such nor that present / return -1") {
	vector<string> wordList = { "qwee", "asd", "asdf", "vxx", "qwe", "123", ",asd" };
	long expected = -1;
	long result = findSuchThat(wordList);

	REQUIRE(expected == result);
}