#include <string>

#include "QPS/parser/QPSParser.h"
#include "catch.hpp"
#include "exceptions/SemanticException.h"
#include "exceptions/SyntaxException.h"

TEST_CASE("parse / given valid string with such that clause / parse into "
          "correct vector of queries") {
    string test = "stmt s; variable v    ;\n     Select v such that Follows (s, 1) ";
    QPSParser qp;
    vector<Query> queries = qp.parse(test);

    vector<Parameter> selectParams{ Parameter("v", ParameterType::VARIABLE) };
    Parameter rel1Param1("s", ParameterType::STMT);
    Parameter rel1Param2("1", ParameterType::FIXED_INT);
    vector<Parameter> rel1Params{ rel1Param1, rel1Param2 };
    vector<shared_ptr<Relationship>> relationships{ Relationship::makeRelationship(AppConstants::FOLLOWS, rel1Params) };
    vector<Pattern> patterns{};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == queries.at(0));
}

TEST_CASE("parse / select tuple, variables not declared / Semantic Error") {
    string test = "stmt s; variable v;\n Select <s ,  g ,  q , asd   ,v> such that Follows (s, 1) ";
    QPSParser qp;
    REQUIRE_THROWS_AS(qp.parse(test), SemanticException);
}

TEST_CASE("parse / no closing > / SyntaxError") {
    string test = "stmt s; variable v;\n Select <s, a such that Follows (s, 1) ";
    QPSParser qp;
    REQUIRE_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / select tuple with spaces should return syntax error / syntax error") {
    string test = "stmt s, abc, def, ghi;\n Select <a b c,d e f, g h i> such that Follows (s, 1) ";
    QPSParser qp;
    REQUIRE_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / select multiple params without tuple / syntax error") {
    string test = "stmt s1, s2;\n Select s1 s2 such that Follows (s1, 1)";
    QPSParser qp;
    REQUIRE_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / given valid string with such that and pattern clause / "
          "parse into correct vector of queries") {
    string test = "stmt s; variable v; assign a   ;\n     Select v such that "
                  "Follows (s, 1) pattern a (v, _\"a + b\"_) ";
    QPSParser qp;
    vector<Query> queries = qp.parse(test);

    vector<Parameter> selectParams{ Parameter("v", ParameterType::VARIABLE) };
    Parameter rel1Param1("s", ParameterType::STMT);
    Parameter rel1Param2("1", ParameterType::FIXED_INT);
    vector<Parameter> rel1Params{ rel1Param1, rel1Param2 };
    vector<shared_ptr<Relationship>> relationships{ Relationship::makeRelationship(AppConstants::FOLLOWS, rel1Params) };
    Parameter pat1Syn("a", ParameterType::ASSIGN);
    Parameter pat1Ent("v", ParameterType::VARIABLE);
    vector<string> pat1Expr{ "_a+b_" };
    vector<Pattern> patterns{ Pattern(pat1Syn, pat1Ent, pat1Expr) };
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == queries.at(0));
}

TEST_CASE("parse / create ifs pattern clause no issues / parse into correct "
          "vector of queries") {
    string test = "variable v; if a; Select v pattern a (v, _, _) ";
    QPSParser qp;
    vector<Query> queries = qp.parse(test);

    vector<Parameter> selectParams{ Parameter("v", ParameterType::VARIABLE) };
    vector<shared_ptr<Relationship>> relationships{};
    Parameter pat1Syn("a", ParameterType::IF);
    Parameter pat1Ent("v", ParameterType::VARIABLE);
    vector<string> pat1Expr{ "_", "_"};
    vector<Pattern> patterns{ Pattern(pat1Syn, pat1Ent, pat1Expr) };
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == queries.at(0));
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

TEST_CASE("parse / wrong design entity / throws syntax error ") {
    string test = "variables v; Select v";
    QPSParser qp;
    CHECK_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / while pattern can only have 2 params (synonym) / throws "
          "syntax error") {
    string test = "variable v; while while; Select v pattern while (v, _, v) ";
    QPSParser qp;
    CHECK_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / ifs pattern can only have 3 params / throws syntax error") {
    string test = "variable v; while while; Select v pattern while (v, _, _, _, _) ";
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

TEST_CASE("parse / parsing undeclared BOOLEAN in select tuple / throw semantic error") {
    string test = "variable v; Select <v, BOOLEAN>";
    QPSParser qp;
    REQUIRE_THROWS_AS(qp.parse(test), SemanticException);
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

TEST_CASE("parse / with clause variable not declared / catch error") {
    string test = "variable v; Select v with a.value = 2";
    QPSParser qp;
    CHECK_THROWS_AS(qp.parse(test), SemanticException);
}

TEST_CASE("parse / with clause variable different comparison types / catch error") {
    string test = "variable v; constant c; Select v with c.value = \"fixedString\"";
    QPSParser qp;
    CHECK_THROWS_AS(qp.parse(test), SemanticException);
}

TEST_CASE("parse / with clause variable invalid attribute / catch error") {
    string test = "variable v; constant c; Select v with c.varName = \"fixedString\"";
    QPSParser qp;
    CHECK_THROWS_AS(qp.parse(test), SemanticException);
}

TEST_CASE("parse / with clause variable empty string / catch syntax error") {
    string test = "variable v; procedure p; Select v with p.procName = \"\"";
    QPSParser qp;
    CHECK_THROWS_AS(qp.parse(test), SyntaxException);
}

TEST_CASE("parse / select <BOOLEAN> should not work / catch semantic error") {
    string query = R"(
        call c;
        variable v;
        Select <BOOLEAN> such that Uses(c,v))";
    QPSParser qp;
    REQUIRE_THROWS_AS(qp.parse(query), SemanticException);
}

TEST_CASE("parse / select <BOOLEAN> should work if BOOLEAN is declared / parse without errors") {
    string query = R"(
        stmt BOOLEAN;
        Select <BOOLEAN> such that Follows(BOOLEAN,2))";
    QPSParser qp;
    vector<Query> queries = qp.parse(query);

    vector<Parameter> selectParams{ Parameter("BOOLEAN", ParameterType::STMT) };
    Parameter rel1Param1("BOOLEAN", ParameterType::STMT);
    Parameter rel1Param2("2", ParameterType::FIXED_INT);
    vector<Parameter> rel1Params{ rel1Param1, rel1Param2 };
    vector<shared_ptr<Relationship>> relationships{ Relationship::makeRelationship(AppConstants::FOLLOWS, rel1Params) };
    vector<Pattern> patterns{};
    vector<Comparison> comparisons{};
    bool isSelectTuple = true;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == queries.at(0));
}

TEST_CASE("parse / select <BOOLEAN, s> should work if BOOLEAN is not declared / catch semantic error") {
    string query = R"(
        stmt s;
        Select <BOOLEAN, s> such that Follows(s,2))";
    QPSParser qp;
    REQUIRE_THROWS_AS(qp.parse(query), SemanticException);
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
        }
        else {
            count += 1;
        }
    }
    CHECK(count == 1);
}

TEST_CASE("splitQuery / given multiple clauses split by ; / vector of string "
          "split by ;") {
    string test = "stmt s; variable v    ;\n     Select v such that Follows (s, 1) ";
    QPSParser qp;
    vector<string> expected = {"stmt s", "variable v", "Select v such that Follows (s, 1)"};
    vector<string> output = qp.splitQuery(test);
    REQUIRE(expected == output);
}

TEST_CASE("checkSynonyms / the variable store contains all the required "
          "variables in query / no exceptions thrown") {

    QPSParser qp;
    VariableStore vs;
    SelectQueryParser sqp;
    Parameter p1("v", ParameterType::VARIABLE);
    Parameter p2("s", ParameterType::STMT);
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
    Parameter p1("v", ParameterType::VARIABLE);
    vs.insertVariable(p1);
    Query q = sqp.parse("Select s such that Modifies(s, v)");
    REQUIRE_THROWS_AS(qp.checkSynonyms(&q, vs), SemanticException);
}

TEST_CASE("checkSynonyms / the variable store does not contain variable / "
          "exception is thrown") {

    QPSParser qp;
    VariableStore vs;
    SelectQueryParser sqp;
    Parameter p1("s", ParameterType::STMT);
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
    Parameter p1("s", ParameterType::STMT);
    Parameter p2("s2", ParameterType::STMT);
    Parameter p3("s3", ParameterType::STMT);
    Parameter p4("s4", ParameterType::STMT);
    Parameter p5("v", ParameterType::VARIABLE);
    Parameter p6("v2", ParameterType::VARIABLE);
    Parameter p7("v3", ParameterType::VARIABLE);
    Parameter p8("v4", ParameterType::VARIABLE);
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
    Parameter p2("s2", ParameterType::STMT);
    Parameter p3("s3", ParameterType::STMT);
    Parameter p4("s4", ParameterType::STMT);
    Parameter p6("v2", ParameterType::VARIABLE);
    Parameter p7("v3", ParameterType::VARIABLE);
    Parameter p8("v4", ParameterType::VARIABLE);
    vs.insertVariable(p2);
    vs.insertVariable(p3);
    vs.insertVariable(p4);
    vs.insertVariable(p6);
    vs.insertVariable(p7);
    vs.insertVariable(p8);
    Query q = sqp.parse("Select s such that Modifies(s, v)");
    REQUIRE_THROWS_AS(qp.checkSynonyms(&q, vs), SemanticException);
}