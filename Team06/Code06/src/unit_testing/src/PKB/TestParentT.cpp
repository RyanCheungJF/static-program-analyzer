#include "catch.hpp"
//#include "../../../spa/src/PKB/storage/ParentTStorage.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"
#include "../utils/utils.h"

using namespace std;
using namespace unit_testing_utils;

TEST_CASE("Check writes and reads to/from ParentTStorage") {
    ParentTStorage pts;
    std::unordered_set<StmtNum> children = {2, 3, 4};
    pts.write(1, children);

    bool res = pts.exists(1, 2);
    REQUIRE(res);

    res = pts.exists(1, 4);
    REQUIRE(res);

    res = pts.exists(1, 5);
    REQUIRE(!res);

    res = pts.exists(3, 2);
    REQUIRE(!res);
}

TEST_CASE("Tests for getting children for ParentTStorage") {
    ParentTStorage pts;
    std::unordered_set<StmtNum> children = {2, 3, 4};
    pts.write(1, children);

    std::unordered_set<StmtNum> res = pts.getRightWildcard(1);
    std::unordered_set<StmtNum> check{ 2, 3, 4 };
    REQUIRE(res == check);

    res = pts.getRightWildcard(2);
    check = {};
    REQUIRE(res == check);

    res = pts.getRightWildcard(3);
    check = {};
    REQUIRE(res == check);
}

TEST_CASE("Tests for getting parent for ParentTStorage") {
    ParentTStorage pts;
    std::unordered_set<StmtNum> children1 = {2, 3, 4};
    std::unordered_set<StmtNum> children2 = {3};
    pts.write(1, children1);
    pts.write(2, children2);

    std::unordered_set<StmtNum> res = pts.getLeftWildcard(3);
    std::unordered_set<StmtNum> check{ 1, 2 };
    REQUIRE(res == check);

    res = pts.getLeftWildcard(1);
    check = {};
    REQUIRE(res == check);

    res = pts.getLeftWildcard(5);
    check = {};
    REQUIRE(res == check);
}

TEST_CASE("Checks for cases e.g. ParentT(assign, 3)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> children1{ 2, 3, 4 };
    unordered_set<StmtNum> children2{ 3 };
    writePkb.setParentT(1, children1);
    writePkb.setParentT(2, children2);
    writePkb.setStatement("assign", 1);
    writePkb.setStatement("assign", 2);
    writePkb.setStatement("if", 3);

    Parameter param1 = Parameter("a", "assign");
    Parameter param2 = Parameter("3", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Parent*", params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 2);
    REQUIRE(contains(res, { "1", "3" }));
    REQUIRE(contains(res, { "2", "3" }));

}

TEST_CASE("Checks for cases e.g. ParentT(while, assign)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> children{ 2, 3, 4 };
    writePkb.setParentT(1, children);
    writePkb.setStatement("while", 1);
    writePkb.setStatement("assign", 2);
    writePkb.setStatement("assign", 3);
    writePkb.setStatement("call", 4);

    Parameter param1 = Parameter("w", "while");
    Parameter param2 = Parameter("a", "assign");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Parent*", params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 2);
    REQUIRE(contains(res, { "1", "2" }));
    REQUIRE(contains(res, { "1", "3" }));
}

TEST_CASE("Checks for cases e.g. Parent(stmt, _)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> children{ 2, 3, 4 };
    writePkb.setParentT(1, children);
    writePkb.setStatement("if", 1);
    writePkb.setStatement("call", 2);
    writePkb.setStatement("assign", 3);
    writePkb.setStatement("print", 4);

    Parameter param1 = Parameter("s", "stmt");
    Parameter param2 = Parameter("_", "wildcard");

    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Parent*", params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 3);
    REQUIRE(contains(res, { "1", "2" }));
    REQUIRE(contains(res, { "1", "3" }));
    REQUIRE(contains(res, { "1", "4" }));
}

TEST_CASE("Checks that if both synonyms are the same, returns empty vector e.g. ParentT(s, s)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> children{ 2, 3, 4 };
    writePkb.setParentT(1, children);
    writePkb.setStatement("if", 1);
    writePkb.setStatement("call", 2);
    writePkb.setStatement("assign", 3);
    writePkb.setStatement("print", 4);

    Parameter param1 = Parameter("s", "stmt");
    Parameter param2 = Parameter("s", "stmt");

    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Parent*", params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 0);
}
