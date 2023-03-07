#include "QPS/QPSGrammarUtils.h"
#include "catch.hpp"
using namespace std;

TEST_CASE("isExprSpec / underscore is valid expr spec / return true") {
    REQUIRE(isExprSpec("_"));
}

TEST_CASE("isExprSpec / empty string is no a valid expr spec / return false") {
    REQUIRE(!isExprSpec(""));
}

TEST_CASE("isExprSpec / _\"a\"_ is valid expr spec / return true") {
    REQUIRE(isExprSpec("_\"a\"_"));
}

TEST_CASE("isExprSpec / \"_a_\" is NOT valid expr spec / return true") {
    // flipping the " and _
    REQUIRE(!isExprSpec("\"_a_\""));
}

TEST_CASE("isExprSpec / _  \"a  \"  _ is valid expr spec / return true") {
    // adding stupid amount of whitespace
    REQUIRE(isExprSpec("_  \"a  \"  _ "));
}

TEST_CASE("isExprSpec / _a_ is NOT a valid expr spec / return false") {
    REQUIRE(!isExprSpec("_a_"));
}

TEST_CASE("isExprSpec / \"a + b\" is a valid expr spec / return true") {
    REQUIRE(isExprSpec("\"a + b\""));
}

TEST_CASE("isExprSpec / a + b is NOT a valid expr spec / return false") {
    // reason is a + b is invalid but "a + b" is valid.
    REQUIRE(!isExprSpec("a + b"));
}

TEST_CASE("isExprSpec / a is a valid expr spec / return true") {
    REQUIRE(isExprSpec("\"a\""));
}

TEST_CASE("isExprSpec / \"a +   b -c * d\" is a valid expr spec / return true") {
    REQUIRE(isExprSpec("\"a +   b -c * d\""));
}

TEST_CASE("isExprSpec / \"a + (a) -  (((b) -c)) * d\" is a valid expr spec / "
          "return true") {
    REQUIRE(isExprSpec("\"a + (a) -  (((b) -c)) * d\""));
}

TEST_CASE("isExprSpec / \"(a)\" is a valid expr spec / return true") {
    REQUIRE(isExprSpec("\"(a)\""));
}

TEST_CASE("isExprSpec / \"(a+((b-c)*(x+d)))\" is a valid expr spec / return true") {
    REQUIRE(isExprSpec("\"(a+((b-c)*(x+d)))\""));
}

// IS EXPR

TEST_CASE("isExpr / empty string is not a valid expr / return false") {
    REQUIRE(!isExpr(""));
}

TEST_CASE("isExpr / a + b is a valid expression / return true") {
    REQUIRE(isExpr("a + b"));
}

TEST_CASE("isExpr / +b is an invalid expression / return false") {
    REQUIRE(!isExpr("+b"));
}

TEST_CASE("isExpr / a+ is an invalid expression / return false") {
    REQUIRE(!isExpr("a+"));
}

// IS FACTOR

TEST_CASE("isFactor / empty string is NOT a valid factor / return false") {
    REQUIRE(!isFactor(""));
}

TEST_CASE("isFactor / (a) is a valid factor / return true") {
    REQUIRE(isFactor("(a)"));
}

TEST_CASE("isFactor / 123 is a valid factor / return true") {
    REQUIRE(isFactor("123"));
}

TEST_CASE("isFactor / 0123 is NOT a valid factor / return false") {
    REQUIRE(!isFactor("0123"));
}

TEST_CASE("isFactor / abc123 is a valid factor / return true") {
    REQUIRE(isFactor("abc123"));
}

TEST_CASE("isFactor / (a+b) is a valid factor / return true") {
    REQUIRE(isFactor("(a+b)"));
}

TEST_CASE("isFactor / ((((a+b)))) is a valid factor / return true") {
    REQUIRE(isFactor("((((a+b))))"));
}

TEST_CASE("isFactor / ((((a+b))))) is NOT a valid factor / return false") {
    // Mismatch of brackets
    REQUIRE(!isFactor("((((a+b)))))"));
}