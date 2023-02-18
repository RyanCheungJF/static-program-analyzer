#include "catch.hpp"
#include "SP/Tokenizer.h"

TEST_CASE("Valid Integers") {
	Tokenizer tokenizer;

	SECTION("Checks for a valid integer") {
		std::string testString = "23";
		REQUIRE(tokenizer.isInteger(testString));
	}

	SECTION("Checks that 0 is valid") {
		std::string testString = "0";
		REQUIRE(tokenizer.isInteger(testString));
	}
}

TEST_CASE("Invalid Integers") {
	Tokenizer tokenizer;

	SECTION("Checks that leading zero is not a valid integer") {
		std::string testString = "023";
		REQUIRE(!tokenizer.isInteger(testString));
	}

	SECTION("Checks that a mix of alphanumeric is not a valid integer") {
		std::string testString = "1test2";
		REQUIRE(!tokenizer.isInteger(testString));
	}

	SECTION("Checks that a mix of symbols is not a valid integer") {
		std::string testString = "23*4";
		REQUIRE(!tokenizer.isInteger(testString));
	}
}