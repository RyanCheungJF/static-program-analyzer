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

TEST_CASE("parse / given valid string with such that clause / parse into correct vector of queries") {
	string test = "stmt s; variable v    ;\n     Select v such that Follows (s, 1) ";
	QPSParser qp;
	vector<Query> queries = qp.parse(test);
	REQUIRE(true);
}

TEST_CASE("parse / given valid string with such that and pattern clause / parse into correct vector of queries") {
	string test = "stmt s; variable v; assign a   ;\n     Select v such that Follows (s, 1) pattern a (v, _\"a + b\"_) ";
	QPSParser qp;
	vector<Query> queries = qp.parse(test);
	REQUIRE(true);
}

TEST_CASE("parse / missing select clause / throw error") {
    string test = "variable v;stmt s;";
    QPSParser qp;
    CHECK_THROWS(qp.parse(test));
}

TEST_CASE("splitQuery / splitting variable v; Select v; should give error / catch error") {
    string test = "variable v; Select v ; ";
    QPSParser qp;
    CHECK_THROWS(qp.splitQuery(test));
}

