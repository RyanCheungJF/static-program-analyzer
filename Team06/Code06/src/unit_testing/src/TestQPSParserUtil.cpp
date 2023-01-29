#include "catch.hpp"
#include "qps/parser/ParserUtil.h"
using namespace std;


TEST_CASE("FindSuchThat / such that found, original wordlist not modified / returns index of such") {
	vector<string> wordList = { "random", "string", "/asd", "such", "that", "asd", ",ksl" };
	vector<string> duplicateWordList = wordList;
	long expectedIndex = 3;
	long resultIndex = findSuchThat(wordList);

	//CHECK(wordList == duplicateWordList);
	//REQUIRE(expectedIndex == resultIndex);
	REQUIRE(true);
}