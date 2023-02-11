#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("Check writes and reads to/from ParentStorage") {
    ParentStorage par;

    par.write(1, 2);
    bool res = par.exists(1, 2);
    REQUIRE(res);

    par.write(1, 2);
    res = par.exists(1, 3);
    REQUIRE(!res);

    par.write(1, 2);
    res = par.exists(3, 2);
    REQUIRE(!res);
}

TEST_CASE("Tests for getting children") {
    ParentStorage par;

    par.write(1, 2);
    par.write(1, 3);

    std::unordered_set<StmtNum> res = par.getRightWildcard(1);
    std::unordered_set<StmtNum> check{ 2, 3 };
    REQUIRE(res == check);

    res = par.getRightWildcard(2);
    check = {};
    REQUIRE(res == check);

    res = par.getRightWildcard(3);
    check = {};
    REQUIRE(res == check);
}

TEST_CASE("Tests for getting parent") {
    ParentStorage par;

    par.write(1, 2);

    std::unordered_set<StmtNum> res = par.getLeftWildcard(2);
    std::unordered_set<StmtNum> check{ 1 };
    REQUIRE(res == check);

    res = par.getLeftWildcard(1);
    check = {};
    REQUIRE(res == check);

    res = par.getLeftWildcard(3);
    check = {};
    REQUIRE(res == check);
}

// Test cases should be done. Waiting on Relationship object to support Parent Relationship
/*
TEST_CASE("Checks for cases e.g. Parent(1, assign)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    ParentStorage ps;
    pkb.parentStorage = &ps;
    StmtStorage sts;
    pkb.statementStorage = &sts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    writePkb.setParent(1, 3);
    writePkb.setStatement("assign", 2);
    writePkb.setStatement("if", 3);

    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("a", "assign");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Parent", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"}, {"1", "3"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks for cases e.g. Parent(while, assign)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    ParentStorage ps;
    pkb.parentStorage = &ps;
    StmtStorage sts;
    pkb.statementStorage = &sts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    writePkb.setParent(1, 3);
    writePkb.setStatement("while", 1);
    writePkb.setStatement("assign", 2);
    writePkb.setStatement("assign", 3);

    Parameter param1 = Parameter("w", "while");
    Parameter param2 = Parameter("if", "if");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Parent", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"}, {"1", "3"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks for cases e.g. Parent(_, stmt)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    ParentStorage ps;
    pkb.parentStorage = &ps;
    StmtStorage sts;
    pkb.statementStorage = &sts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    writePkb.setParent(1, 3);
    writePkb.setStatement("call", 2);
    writePkb.setStatement("assign", 3);

    Parameter param1 = Parameter("_", "wildcard");
    Parameter param2 = Parameter("s", "stmt");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Parent", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"}, {"1", "3"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}
*/
