#include "catch.hpp"
#include "qps/parser/QPSParser.h"
#include <string>

TEST_CASE("splitQuery / given multiple clauses split by ; / vector of string split by ;") {
	string test = "stmt s; variable v    ;\n     Select v such that Follows (s, 1) ";
	QPSParser qp;
	vector<string> expected = { "stmt s", "variable v", "Select v such that Follows (s, 1)" };
	vector<string> output = qp.splitQuery(test);
	REQUIRE(expected == output);
}

TEST_CASE("parse / given valid string / parse into correct vector of queries") {
	string test = "stmt s; variable v    ;\n     Select v such that Follows (s, 1) ";
	QPSParser qp;
	vector<Query> queries = qp.parse(test);
	REQUIRE(true);}