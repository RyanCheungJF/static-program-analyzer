#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("Check writes and reads to/from ParentTStorage") {
    ParentTStorage par;

    std::vector<std::pair<StmtNum, StmtNum>> parentChild;
    parentChild.push_back({ 1, 2 });
    parentChild.push_back({ 1, 3 });
    parentChild.push_back({ 1, 4 });
    par.write(parentChild);

    bool res = par.exists(1, 2);
    REQUIRE(res);

    res = par.exists(1, 4);
    REQUIRE(res);

    res = par.exists(1, 5);
    REQUIRE(!res);

    res = par.exists(3, 2);
    REQUIRE(!res);
}

TEST_CASE("Tests for getting children for ParentTStorage") {
    ParentTStorage par;

    std::vector<std::pair<StmtNum, StmtNum>> parentChild;
    parentChild.push_back({ 1, 2 });
    parentChild.push_back({ 1, 3 });
    parentChild.push_back({ 1, 4 });
    par.write(parentChild);

    std::unordered_set<StmtNum> res = par.getRightWildcard(1);
    std::unordered_set<StmtNum> check{ 2, 3, 4 };
    REQUIRE(res == check);

    res = par.getRightWildcard(2);
    check = {};
    REQUIRE(res == check);

    res = par.getRightWildcard(3);
    check = {};
    REQUIRE(res == check);
}

TEST_CASE("Tests for getting parent for ParentTStorage") {
    ParentTStorage par;

    std::vector<std::pair<StmtNum, StmtNum>> parentChild;
    parentChild.push_back({ 1, 2 });
    parentChild.push_back({ 2, 3 });
    parentChild.push_back({ 1, 3 });
    parentChild.push_back({ 1, 4 });
    par.write(parentChild);

    std::unordered_set<StmtNum> res = par.getLeftWildcard(3);
    std::unordered_set<StmtNum> check{ 1, 2 };
    REQUIRE(res == check);

    res = par.getLeftWildcard(1);
    check = {};
    REQUIRE(res == check);

    res = par.getLeftWildcard(5);
    check = {};
    REQUIRE(res == check);
}

// Test cases should be done. Waiting on Relationship object to support ParentT Relationship
/*
TEST_CASE("Checks for cases e.g. ParentT(assign, 3)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> parentChildPairs;
    parentChildPairs.push_back({ 1, 2 });
    parentChildPairs.push_back({ 2, 3 });
    parentChildPairs.push_back({ 1, 3 });
    parentChildPairs.push_back({ 1, 4 });
    writePkb.setParentT(parentChildPairs);
    writePkb.setStatement("assign", 1);
    writePkb.setStatement("assign", 2);
    writePkb.setStatement("if", 3);

    Parameter param1 = Parameter("a", "assign");
    Parameter param2 = Parameter("3", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Parent*", params);

    std::vector<std::vector<std::string>> check = { {"1", "3"}, {"2", "3"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks for cases e.g. ParentT(while, assign)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> parentChildPairs;
    parentChildPairs.push_back({ 1, 2 });
    parentChildPairs.push_back({ 1, 3 });
    parentChildPairs.push_back({ 1, 4 });
    writePkb.setParentT(parentChildPairs);
    writePkb.setStatement("while", 1);
    writePkb.setStatement("assign", 2);
    writePkb.setStatement("assign", 3);
    writePkb.setStatement("call", 4);

    Parameter param1 = Parameter("w", "while");
    Parameter param2 = Parameter("a", "assign");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Parent*", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"}, {"1", "3"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks for cases e.g. Parent(stmt, _)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> parentChildPairs;
    parentChildPairs.push_back({ 1, 2 });
    parentChildPairs.push_back({ 1, 3 });
    parentChildPairs.push_back({ 1, 4 });
    writePkb.setParentT(parentChildPairs);
    writePkb.setStatement("if", 1);
    writePkb.setStatement("call", 2);
    writePkb.setStatement("assign", 3);
    writePkb.setStatement("print", 4);

    Parameter param1 = Parameter("s", "stmt");
    Parameter param2 = Parameter("_", "wildcard");

    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Parent*", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"}, {"1", "3"}, {"1", "4"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks that if both synonyms are the same, returns empty vector e.g. ParentT(s, s)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> parentChildPairs;
    parentChildPairs.push_back({ 1, 2 });
    parentChildPairs.push_back({ 1, 3 });
    parentChildPairs.push_back({ 1, 4 });
    writePkb.setParentT(parentChildPairs);
    writePkb.setStatement("if", 1);
    writePkb.setStatement("call", 2);
    writePkb.setStatement("assign", 3);
    writePkb.setStatement("print", 4);

    Parameter param1 = Parameter("s", "stmt");
    Parameter param2 = Parameter("s", "stmt");

    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Parent*", params);

    std::vector<std::vector<std::string>> check = {};
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}
*/