#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"
#include "../utils/utils.h"

using namespace unit_testing_utils;

TEST_CASE("Check writes and reads to/from ParentStorage") {
    ParentStorage par;

    par.write(1, 2);
    REQUIRE(par.exists(1, 2));

    par.write(1, 2);
    REQUIRE(!par.exists(1, 3));

    par.write(1, 2);
    REQUIRE(!par.exists(3, 2));
}

TEST_CASE("Tests for getting children") {
    ParentStorage par;

    par.write(1, 2);
    par.write(1, 3);

    std::unordered_set<StmtNum> res = par.getRightWildcard(1);
    std::unordered_set<StmtNum> check{ 2, 3 };
    REQUIRE(unit_testing_utils::equals(check, res));

    res = par.getRightWildcard(2);
    REQUIRE(res.empty());

    res = par.getRightWildcard(3);
    REQUIRE(res.empty());
}

TEST_CASE("Tests for getting parent") {
    ParentStorage par;

    par.write(1, 2);

    std::unordered_set<StmtNum> res = par.getLeftWildcard(2);
    std::unordered_set<StmtNum> check{ 1 };
    REQUIRE(unit_testing_utils::equals(check, res));

    res = par.getLeftWildcard(1);
    REQUIRE(res.empty());

    res = par.getLeftWildcard(3);
    REQUIRE(res.empty());
}

TEST_CASE("Checks for cases e.g. Parent(1, assign)") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    writePkb.setParent(1, 3);
    writePkb.setStatement(CONSTANTS.ASSIGN, 2);
    writePkb.setStatement(CONSTANTS.IF, 3);

    Parameter param1 = Parameter("1", CONSTANTS.FIXED_INT);
    Parameter param2 = Parameter("a", CONSTANTS.ASSIGN);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.PARENT, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 1);
    REQUIRE(contains(res, { "1", "2" }));
}

TEST_CASE("Checks for cases e.g. Parent(while, assign)") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    writePkb.setParent(1, 3);
    writePkb.setStatement(CONSTANTS.WHILE, 1);
    writePkb.setStatement(CONSTANTS.ASSIGN, 2);
    writePkb.setStatement(CONSTANTS.IF, 3);

    Parameter param1 = Parameter("w", CONSTANTS.WHILE);
    Parameter param2 = Parameter(CONSTANTS.IF, CONSTANTS.IF);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.PARENT, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 1);
    REQUIRE(contains(res, { "1", "3" }));
}

TEST_CASE("Checks for cases e.g. Parent(_, stmt)") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    writePkb.setParent(1, 3);
    writePkb.setStatement(CONSTANTS.CALL, 2);
    writePkb.setStatement(CONSTANTS.ASSIGN, 3);

    Parameter param1 = Parameter("_", CONSTANTS.WILDCARD);
    Parameter param2 = Parameter("s", CONSTANTS.STMT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.PARENT, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 2);
    REQUIRE(contains(res, { "1", "2" }));
    REQUIRE(contains(res, { "1", "3" }));
}
