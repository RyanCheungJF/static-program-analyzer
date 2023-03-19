#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;

TEST_CASE("Check writes and reads to/from ParentStorage") {
    RelationshipStorage<StmtNum, StmtNum> par;

    par.write(1, 2);
    REQUIRE(par.exists(1, 2));

    par.write(1, 2);
    REQUIRE(!par.exists(1, 3));

    par.write(1, 2);
    REQUIRE(!par.exists(3, 2));
}

TEST_CASE("Tests for getting children") {
    RelationshipStorage<StmtNum, StmtNum> par;

    par.write(1, 2);
    par.write(1, 3);

    std::unordered_set<StmtNum> res = par.getRightItems(1);
    std::unordered_set<StmtNum> check{2, 3};
    REQUIRE(unit_testing_utils::equals(check, res));

    res = par.getRightItems(2);
    REQUIRE(res.empty());

    res = par.getRightItems(3);
    REQUIRE(res.empty());
}

TEST_CASE("Tests for getting parent") {
    RelationshipStorage<StmtNum, StmtNum> par;

    par.write(1, 2);

    std::unordered_set<StmtNum> res = par.getLeftItems(2);
    std::unordered_set<StmtNum> check{1};
    REQUIRE(unit_testing_utils::equals(check, res));

    res = par.getLeftItems(1);
    REQUIRE(res.empty());

    res = par.getLeftItems(3);
    REQUIRE(res.empty());
}

TEST_CASE("Checks for cases e.g. Parent(1, assign)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    writePkb.setParent(1, 3);
    writePkb.setStatement(AppConstants::ASSIGN, 2);
    writePkb.setStatement(AppConstants::IF, 3);

    Parameter param1 = Parameter("1", ParameterType::FIXED_INT);
    Parameter param2 = Parameter("a", ParameterType::ASSIGN);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::PARENT, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 1);
    REQUIRE(contains(res, {"1", "2"}));
}

TEST_CASE("Checks for cases e.g. Parent(while, assign)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    writePkb.setParent(1, 3);
    writePkb.setStatement(AppConstants::WHILE, 1);
    writePkb.setStatement(AppConstants::ASSIGN, 2);
    writePkb.setStatement(AppConstants::IF, 3);

    Parameter param1 = Parameter("w", ParameterType::WHILE);
    Parameter param2 = Parameter("if", ParameterType::IF);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::PARENT, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 1);
    REQUIRE(contains(res, {"1", "3"}));
}

TEST_CASE("Checks for cases e.g. Parent(_, stmt)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    writePkb.setParent(1, 3);
    writePkb.setStatement(AppConstants::CALL, 2);
    writePkb.setStatement(AppConstants::ASSIGN, 3);

    Parameter param1 = Parameter("_", ParameterType::WILDCARD);
    Parameter param2 = Parameter("s", ParameterType::STMT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::PARENT, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 2);
    REQUIRE(contains(res, {"1", "2"}));
    REQUIRE(contains(res, {"1", "3"}));
}
