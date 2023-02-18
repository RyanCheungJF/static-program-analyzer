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
    CHECK_THROWS_AS(qp.splitQuery(test), SyntaxException);
}

TEST_CASE("checkSynonyms / the variable store contains all the required variables in query / no exceptions thrown") {
    QPSParser qp;
    VariableStore vs;
    SelectQueryParser sqp;
    Parameter p1("v", "variable");
    Parameter p2("s", "stmt");
    vs.insertVariable(p1);
    vs.insertVariable(p2);
    Query q = sqp.parse("Select s such that Modifies(s, v)");
    REQUIRE_NOTHROW(qp.checkSynonyms(&q, vs));
}

TEST_CASE("checkSynonyms / the variable store does not contain stmt / exception is thrown") {
    QPSParser qp;
    VariableStore vs;
    SelectQueryParser sqp;
    Parameter p1("v", "variable");
    vs.insertVariable(p1);
    Query q = sqp.parse("Select s such that Modifies(s, v)");
    REQUIRE_THROWS(qp.checkSynonyms(&q, vs));
}

TEST_CASE("checkSynonyms / the variable store does not contain variable / exception is thrown") {
    QPSParser qp;
    VariableStore vs;
    SelectQueryParser sqp;
    Parameter p1("s", "stmt");
    vs.insertVariable(p1);
    Query q = sqp.parse("Select s such that Modifies(s, v)");
    REQUIRE_THROWS(qp.checkSynonyms(&q, vs));
}

TEST_CASE("checkSynonyms / variable store is empty / exception is thrown") {
    QPSParser qp;
    VariableStore vs;
    SelectQueryParser sqp;
    Query q = sqp.parse("Select s");
    REQUIRE_THROWS(qp.checkSynonyms(&q, vs));
}

TEST_CASE("checkSynonyms / the variable store contains more variable than needed / no exception is thrown") {
    QPSParser qp;
    VariableStore vs;
    SelectQueryParser sqp;
    Parameter p1("s", "stmt");
    Parameter p2("s2", "stmt");
    Parameter p3("s3", "stmt");
    Parameter p4("s4", "stmt");
    Parameter p5("v", "variable");
    Parameter p6("v2", "variable");
    Parameter p7("v3", "variable");
    Parameter p8("v4", "variable");
    vs.insertVariable(p1);
    vs.insertVariable(p2);
    vs.insertVariable(p3);
    vs.insertVariable(p4);
    vs.insertVariable(p5);
    vs.insertVariable(p6);
    vs.insertVariable(p7);
    vs.insertVariable(p8);
    Query q = sqp.parse("Select s such that Modifies(s, v)");
    REQUIRE_NOTHROW(qp.checkSynonyms(&q, vs));
}

TEST_CASE("checkSynonyms / the variable store has correct type but wrong synonym / exception is thrown") {
    QPSParser qp;
    VariableStore vs;
    SelectQueryParser sqp;
    Parameter p2("s2", "stmt");
    Parameter p3("s3", "stmt");
    Parameter p4("s4", "stmt");
    Parameter p6("v2", "variable");
    Parameter p7("v3", "variable");
    Parameter p8("v4", "variable");
    vs.insertVariable(p2);
    vs.insertVariable(p3);
    vs.insertVariable(p4);
    vs.insertVariable(p6);
    vs.insertVariable(p7);
    vs.insertVariable(p8);
    Query q = sqp.parse("Select s such that Modifies(s, v)");
    REQUIRE_THROWS(qp.checkSynonyms(&q, vs));
}