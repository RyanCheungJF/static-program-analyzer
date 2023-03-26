#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;

TEST_CASE("Check writes and reads to/from ParentTStorage") {
    RelationshipStorage<StmtNum, StmtNum> pts;
    std::unordered_set<StmtNum> children = {2, 3, 4};
    pts.write(1, children);

    REQUIRE(pts.exists(1, 2));
    REQUIRE(pts.exists(1, 4));
    REQUIRE(!pts.exists(1, 5));
    REQUIRE(!pts.exists(3, 2));
}

TEST_CASE("Tests for getting children for ParentTStorage") {
    RelationshipStorage<StmtNum, StmtNum> pts;
    std::unordered_set<StmtNum> children = {2, 3, 4};
    pts.write(1, children);

    std::unordered_set<StmtNum> res = *pts.getRightItems(1);
    std::unordered_set<StmtNum> check{2, 3, 4};
    REQUIRE(unit_testing_utils::equals(check, res));

    res = *pts.getRightItems(2);
    REQUIRE(res.empty());

    res = *pts.getRightItems(3);
    REQUIRE(res.empty());
}

TEST_CASE("Tests for getting parent for ParentTStorage") {
    RelationshipStorage<StmtNum, StmtNum> pts;
    std::unordered_set<StmtNum> children1 = {2, 3, 4};
    std::unordered_set<StmtNum> children2 = {3};
    pts.write(1, children1);
    pts.write(2, children2);

    std::unordered_set<StmtNum> res = *pts.getLeftItems(3);
    std::unordered_set<StmtNum> check{1, 2};
    REQUIRE(unit_testing_utils::equals(check, res));

    res = *pts.getLeftItems(1);
    REQUIRE(res.empty());

    res = *pts.getLeftItems(5);
    REQUIRE(res.empty());
}

TEST_CASE("Checks for cases e.g. ParentT(assign, 3)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> children1{2, 3, 4};
    unordered_set<StmtNum> children2{3};
    writePkb.setParentT(1, children1);
    writePkb.setParentT(2, children2);
    writePkb.setStatement(AppConstants::ASSIGN, 1);
    writePkb.setStatement(AppConstants::ASSIGN, 2);
    writePkb.setStatement(AppConstants::IF, 3);

    Parameter param1 = Parameter("a", ParameterType::ASSIGN);
    Parameter param2 = Parameter("3", ParameterType::FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::PARENTT, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    std::vector<std::vector<std::string>> expected = {{"1", "3"}, {"2", "3"}};
    REQUIRE(unit_testing_utils::equals(expected, res));
}

TEST_CASE("Checks for cases e.g. ParentT(while, assign)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> children{2, 3, 4};
    writePkb.setParentT(1, children);
    writePkb.setStatement(AppConstants::WHILE, 1);
    writePkb.setStatement(AppConstants::ASSIGN, 2);
    writePkb.setStatement(AppConstants::ASSIGN, 3);
    writePkb.setStatement(AppConstants::CALL, 4);

    Parameter param1 = Parameter("w", ParameterType::WHILE);
    Parameter param2 = Parameter("a", ParameterType::ASSIGN);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::PARENTT, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    std::vector<std::vector<std::string>> expected = {{"1", "3"}, {"1", "2"}};
    REQUIRE(unit_testing_utils::equals(expected, res));
}

TEST_CASE("Checks for cases e.g. Parent(stmt, _)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> children{2, 3, 4};
    writePkb.setParentT(1, children);
    writePkb.setStatement(AppConstants::IF, 1);
    writePkb.setStatement(AppConstants::CALL, 2);
    writePkb.setStatement(AppConstants::ASSIGN, 3);
    writePkb.setStatement(AppConstants::PRINT, 4);

    Parameter param1 = Parameter("s", ParameterType::STMT);
    Parameter param2 = Parameter("_", ParameterType::WILDCARD);

    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::PARENTT, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    std::vector<std::vector<std::string>> expected = {{"1", "2"}, {"1", "3"}, {"1", "4"}};
    REQUIRE(unit_testing_utils::equals(expected, res));
}

TEST_CASE("Checks that if both synonyms are the same, returns empty vector e.g. ParentT(s, s)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> children{2, 3, 4};
    writePkb.setParentT(1, children);
    writePkb.setStatement(AppConstants::IF, 1);
    writePkb.setStatement(AppConstants::CALL, 2);
    writePkb.setStatement(AppConstants::ASSIGN, 3);
    writePkb.setStatement(AppConstants::PRINT, 4);

    Parameter param1 = Parameter("s", ParameterType::STMT);
    Parameter param2 = Parameter("s", ParameterType::STMT);

    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::PARENTT, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.empty());
}
