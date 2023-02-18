#include "catch.hpp"
#include <string>
#include "qps/parser/SelectQueryParser.h"
using namespace std;

TEST_CASE("parse / sample select query string / returns query object") {
	string input = "Select s     such    that    Follows(sass,11234)";
	SelectQueryParser sqp;
	Query q = sqp.parse(input);
	CHECK(true);
}

TEST_CASE("parse / Follows query does not have enough params / throws error") {
    string input = "Select s     such    that    Follows(sass)";
    SelectQueryParser sqp;
    CHECK_THROWS(sqp.parse(input));
}

TEST_CASE("parse / Follows query has too many params / throws error") {
    string input = "Select s     such    that    Follows(sass, asd, dad, doo)";
    SelectQueryParser sqp;
    CHECK_THROWS(sqp.parse(input));
}

TEST_CASE("parse / Rel ref is nonsense / throws error") {
    string input = "Select s     such    that  Teaches(sass, asd, dad, doo)";
    SelectQueryParser sqp;
    CHECK_THROWS(sqp.parse(input));
}

TEST_CASE("parse / empty string select clause / throws exception") {
    string input = " ";
    SelectQueryParser sqp;
    CHECK_THROWS(sqp.parse(input));

}

TEST_CASE("parse / nonsense string select clause / throws exception") {
    string input = "Hello World";
    SelectQueryParser sqp;
    CHECK_THROWS(sqp.parse(input));
}

TEST_CASE("parse / small s select clause / throws exception") {
    string input = "select v";
    SelectQueryParser sqp;
    CHECK_THROWS(sqp.parse(input));
}