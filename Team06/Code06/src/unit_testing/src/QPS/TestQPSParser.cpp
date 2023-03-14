#include "catch.hpp"
#include "exceptions/SemanticException.h"
#include "exceptions/SyntaxException.h"
#include "QPS/parser/QPSParser.h"
#include <string>

TEST_CASE("parse / given valid string with such that clause / parse into "
          "correct vector of queries") {
  string test =
      "stmt s; variable v    ;\n     Select v such that Follows (s, 1) ";
  QPSParser qp;
  vector<Query> queries = qp.parse(test);
  REQUIRE(true);
}

TEST_CASE("parse / given valid string with such that and pattern clause / "
          "parse into correct vector of queries") {
  string test = "stmt s; variable v; assign a   ;\n     Select v such that "
                "Follows (s, 1) pattern a (v, _\"a + b\"_) ";
  QPSParser qp;
  vector<Query> queries = qp.parse(test);
  REQUIRE(true);
}

TEST_CASE("parse / create ifs pattern clause no issues / parse into correct "
          "vector of queries") {
  string test = "variable v; if a; Select v pattern a (v, _, _) ";
  QPSParser qp;
  vector<Query> queries = qp.parse(test);
  REQUIRE(true);
}

TEST_CASE("parse / create ifs pattern clause last param is not a wildcard / "
          "throws syntax error") {
  string test = "variable v; if a; Select v pattern a (v, _, \"apple\") ";
  QPSParser qp;
  CHECK_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / assign pattern can only have 2 params (wild card) / throws "
          "semantic error") {
  string test = "variable v; assign a; Select v pattern a (v, _, _) ";
  QPSParser qp;
  CHECK_THROWS_AS(qp.parse(test), SemanticException);
}

TEST_CASE("parse / assign pattern can only have 2 params (synonym) / throws "
          "syntax error") {
  string test = "variable v; assign a; Select v pattern a (v, _, v) ";
  QPSParser qp;
  CHECK_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / while pattern can only have 2 params (wild card) / throws "
          "semantic error") {
  string test = "variable v; while while; Select v pattern while (v, _, _) ";
  QPSParser qp;
  CHECK_THROWS_AS(qp.parse(test), SemanticException);
}

TEST_CASE("parse / while pattern can only have 2 params (synonym) / throws "
          "syntax error") {
  string test = "variable v; while while; Select v pattern while (v, _, v) ";
  QPSParser qp;
  CHECK_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / ifs pattern can only have 3 params / throws syntax error") {
  string test =
      "variable v; while while; Select v pattern while (v, _, _, _, _) ";
  QPSParser qp;
  CHECK_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / additional brackets / throws syntax error") {
  string test = "variable v; while while; Select v pattern while ((v, _)) ";
  QPSParser qp;
  CHECK_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / create while pattern clause second param is not a wildcard "
          "/ throws semantic error") {
  string test = "variable v; while a; Select v pattern a (v, \"apple\") ";
  QPSParser qp;
  CHECK_THROWS_AS(qp.parse(test), SemanticException);
}

TEST_CASE("parse / missing select clause / throw error") {
  string test = "variable v;stmt s;";
  QPSParser qp;
  CHECK_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / parsing empty strings / throw syntax error") {
  string test = "variable v; ; Select v";
  QPSParser qp;
  REQUIRE_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / rubbish clauses is a syntax error / catch error") {
  string test = "coding at 2am sucks; but here is a syntax error;";
  QPSParser qp;
  CHECK_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / variable v; Select v such that Uses(_,_) / catch error") {
  string test = "variable v; Select v such that Uses(_,_)";
  QPSParser qp;
  CHECK_THROWS_AS(qp.parse(test), SemanticException);
}

TEST_CASE("splitQuery / splitting variable v; Select v; should give error / "
          "catch error") {
  string test = "variable v; Select v ; ";
  QPSParser qp;
  CHECK_THROWS_AS(qp.splitQuery(test), SyntaxException);
}

TEST_CASE("splitQuery / splitting variable v;;Select c / still splits normally "
          "with empty string") {
  string test = "variable v; ;Select c  ";
  QPSParser qp;
  vector<string> vs = qp.splitQuery(test);
  CHECK(vs.size() == 3);
}

TEST_CASE("splitQuery / splitting ;;;;;;;;s / still splits normally with 9 "
          "empty strings") {
  string test = ";;;;;;;;s";
  QPSParser qp;
  vector<string> vs = qp.splitQuery(test);
  int count = 0;
  for (string s : vs) {
    if (s != "s") {
      CHECK(s.empty());
    } else {
      count += 1;
    }
  }
  CHECK(count == 1);
}

TEST_CASE("splitQuery / given multiple clauses split by ; / vector of string "
          "split by ;") {
  string test =
      "stmt s; variable v    ;\n     Select v such that Follows (s, 1) ";
  QPSParser qp;
  vector<string> expected = {"stmt s", "variable v",
                             "Select v such that Follows (s, 1)"};
  vector<string> output = qp.splitQuery(test);
  REQUIRE(expected == output);
}

TEST_CASE("checkSynonyms / the variable store contains all the required "
          "variables in query / no exceptions thrown") {

  QPSParser qp;
  VariableStore vs;
  SelectQueryParser sqp;
  Parameter p1("v", AppConstants::VARIABLE);
  Parameter p2("s", AppConstants::STMT);
  vs.insertVariable(p1);
  vs.insertVariable(p2);
  Query q = sqp.parse("Select s such that Modifies(s, v)");
  REQUIRE_NOTHROW(qp.checkSynonyms(&q, vs));
}

TEST_CASE("checkSynonyms / the variable store does not contain stmt / "
          "exception is thrown") {

  QPSParser qp;
  VariableStore vs;
  SelectQueryParser sqp;
  Parameter p1("v", AppConstants::VARIABLE);
  vs.insertVariable(p1);
  Query q = sqp.parse("Select s such that Modifies(s, v)");
  REQUIRE_THROWS_AS(qp.checkSynonyms(&q, vs), SemanticException);
}

TEST_CASE("checkSynonyms / the variable store does not contain variable / "
          "exception is thrown") {

  QPSParser qp;
  VariableStore vs;
  SelectQueryParser sqp;
  Parameter p1("s", AppConstants::STMT);
  vs.insertVariable(p1);
  Query q = sqp.parse("Select s such that Modifies(s, v)");
  REQUIRE_THROWS_AS(qp.checkSynonyms(&q, vs), SemanticException);
}

TEST_CASE("checkSynonyms / variable store is empty / exception is thrown") {
  QPSParser qp;
  VariableStore vs;
  SelectQueryParser sqp;
  Query q = sqp.parse("Select s");
  REQUIRE_THROWS_AS(qp.checkSynonyms(&q, vs), SemanticException);
}

TEST_CASE("checkSynonyms / the variable store contains more variable than "
          "needed / no exception is thrown") {

  QPSParser qp;
  VariableStore vs;
  SelectQueryParser sqp;
  Parameter p1("s", AppConstants::STMT);
  Parameter p2("s2", AppConstants::STMT);
  Parameter p3("s3", AppConstants::STMT);
  Parameter p4("s4", AppConstants::STMT);
  Parameter p5("v", AppConstants::VARIABLE);
  Parameter p6("v2", AppConstants::VARIABLE);
  Parameter p7("v3", AppConstants::VARIABLE);
  Parameter p8("v4", AppConstants::VARIABLE);
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

TEST_CASE("checkSynonyms / the variable store has correct type but wrong "
          "synonym / exception is thrown") {

  QPSParser qp;
  VariableStore vs;
  SelectQueryParser sqp;
  Parameter p2("s2", AppConstants::STMT);
  Parameter p3("s3", AppConstants::STMT);
  Parameter p4("s4", AppConstants::STMT);
  Parameter p6("v2", AppConstants::VARIABLE);
  Parameter p7("v3", AppConstants::VARIABLE);
  Parameter p8("v4", AppConstants::VARIABLE);
  vs.insertVariable(p2);
  vs.insertVariable(p3);
  vs.insertVariable(p4);
  vs.insertVariable(p6);
  vs.insertVariable(p7);
  vs.insertVariable(p8);
  Query q = sqp.parse("Select s such that Modifies(s, v)");
  REQUIRE_THROWS_AS(qp.checkSynonyms(&q, vs), SemanticException);
}