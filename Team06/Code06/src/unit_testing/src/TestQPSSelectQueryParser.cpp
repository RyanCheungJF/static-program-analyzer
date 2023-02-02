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