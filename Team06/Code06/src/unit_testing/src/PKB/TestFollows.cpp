#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"
#include "../utils/utils.h"

using namespace unit_testing_utils;


TEST_CASE("FollowsStorage: write(StmtNum followee, StmtNum follower)") {
    FollowsStorage fs;
    AppConstants CONSTANTS;

    SECTION("exists(StmtNum followee, StmtNum follower): empty storage") {
        REQUIRE(!fs.exists(1, 2));
    }

    SECTION("getRightWildcard(StmtNum leftStmtNum): empty storage") {
        std::unordered_set<StmtNum> res = fs.getRightWildcard(1);
        REQUIRE(res.empty());
    }

    SECTION("getLeftWildcard(StmtNum leftStmtNum): empty storage") {
        std::unordered_set<StmtNum> res = fs.getLeftWildcard(2);
        REQUIRE(res.size() == 0);
    }

    fs.write(1, 2);
    fs.write(2, 3);

    SECTION("exists(StmtNum followee, StmtNum follower): non-empty storage") {
        REQUIRE(fs.exists(1, 2));
        REQUIRE(!fs.exists(1, 3));
        REQUIRE(!fs.exists(3, 4));
    }

    SECTION("getRightWildcard(StmtNum leftStmtNum): non-empty storage") {
        std::vector<StmtNum> expected = { 2 };
        std::unordered_set<StmtNum> res = fs.getRightWildcard(1);
        REQUIRE(unit_testing_utils::equals(expected, res));
    }

    SECTION("getLeftWildcard(StmtNum leftStmtNum): non-empty storage") {
        std::vector<StmtNum> expected = { 1 };
        std::unordered_set<StmtNum> res = fs.getLeftWildcard(2);
        REQUIRE(unit_testing_utils::equals(expected, res));
    }

}

TEST_CASE("Checks FollowsStorage such that given a followee, if it does not have a certain follower, an empty vector is returned") {
    FollowsStorage fs;
    AppConstants CONSTANTS;
    fs.write(1, 2);
    std::unordered_set<StmtNum> res = fs.getRightWildcard(3);
    REQUIRE(res.empty());
}

TEST_CASE("Checks FollowsStorage such that given a follower, if it does not have a certain followee, an empty vector is returned") {
    FollowsStorage fs;
    AppConstants CONSTANTS;
    fs.write(1, 2);
    std::unordered_set<StmtNum> res = fs.getLeftWildcard(3);
    REQUIRE(res.empty());
}

TEST_CASE("Checks that writing and reading from ReadPKB works for Follows") {
    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    Parameter param1 = Parameter("1", CONSTANTS.FIXED_INT);
    Parameter param2 = Parameter("2", CONSTANTS.FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWS, params);

    std::vector<std::vector<std::string>> expected = { {"1", "2"} };
    std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs);
    REQUIRE(unit_testing_utils::equals(expected, actual));
}

TEST_CASE("Checks that a non-existent Follows relationship returns an empty vector from ReadPKB") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    Parameter param1 = Parameter("1", CONSTANTS.FIXED_INT);
    Parameter param2 = Parameter("3", CONSTANTS.FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWS, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.empty());
}

TEST_CASE("Checks that given a followee and a synonym, ReadPKB returns relevant followee-follower pairs") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    writePkb.setStatement(CONSTANTS.ASSIGN, 2);

    Parameter param1 = Parameter("1", CONSTANTS.FIXED_INT);
    Parameter param2 = Parameter("a", CONSTANTS.ASSIGN);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWS, params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(unit_testing_utils::equals(check, res));
}

TEST_CASE("Checks that given a followee and a wildcard, ReadPKB returns all followee-follower pairs") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    Parameter param1 = Parameter("1", CONSTANTS.FIXED_INT);
    Parameter param2 = Parameter("_", CONSTANTS.WILDCARD);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWS, params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(unit_testing_utils::equals(check, res));
}

TEST_CASE("Checks that given a synonym and a follower, ReadPKB returns relevant followee-follower pairs") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    writePkb.setStatement(CONSTANTS.WHILE, 1);

    Parameter param1 = Parameter("w", CONSTANTS.WHILE);
    Parameter param2 = Parameter("2", CONSTANTS.FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWS, params);

    std::vector<std::vector<std::string>> check = { {"1", "2"}};
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(unit_testing_utils::equals(check, res));
}

TEST_CASE("Checks that given a synonym and a synonym, ReadPKB returns relevant followee-follower pairs") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    writePkb.setStatement(CONSTANTS.WHILE, 1);
    writePkb.setStatement(CONSTANTS.IF, 2);

    Parameter param1 = Parameter("w", CONSTANTS.WHILE);
    Parameter param2 = Parameter(CONSTANTS.IF, CONSTANTS.IF);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWS, params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(unit_testing_utils::equals(check, res));
}

TEST_CASE("Checks that given a synonym and a wildcard, ReadPKB returns relevant followee-follower pairs") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    writePkb.setStatement(CONSTANTS.WHILE, 1);
    writePkb.setStatement(CONSTANTS.ASSIGN, 2);

    Parameter param1 = Parameter("w", CONSTANTS.WHILE);
    Parameter param2 = Parameter("_", CONSTANTS.WILDCARD);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWS, params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(unit_testing_utils::equals(check, res));
}

TEST_CASE("Checks that given a wildcard and a follower, ReadPKB returns relevant followee-follower pairs") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);

    Parameter param1 = Parameter("_", CONSTANTS.WILDCARD);
    Parameter param2 = Parameter("2", CONSTANTS.FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWS, params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(unit_testing_utils::equals(check, res));
}

TEST_CASE("Checks that given a wildcard and a synonym, ReadPKB returns relevant followee-follower pairs") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    writePkb.setStatement(CONSTANTS.PRINT, 2);

    Parameter param1 = Parameter("_", CONSTANTS.WILDCARD);
    Parameter param2 = Parameter("p", CONSTANTS.PRINT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWS, params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(unit_testing_utils::equals(check, res));
}

TEST_CASE("Checks that given a wildcard and a wildcard, ReadPKB returns relevant followee-follower pairs") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);

    Parameter param1 = Parameter("_", CONSTANTS.WILDCARD);
    Parameter param2 = Parameter("_", CONSTANTS.WILDCARD);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWS, params);

    std::vector<std::vector<std::string>> check = { {"1", "2"}};
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(unit_testing_utils::equals(check, res));
}

TEST_CASE("Checks that given a followee, if it does not have a certain follower, readPKB returns an empty vector") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    Parameter param1 = Parameter("2", CONSTANTS.FIXED_INT);
    Parameter param2 = Parameter("_", CONSTANTS.WILDCARD);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWS, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.empty());
}

TEST_CASE("Checks that given a follower, if it does not have a certain followee, readPKB returns an empty vector") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    Parameter param1 = Parameter("_", CONSTANTS.WILDCARD);
    Parameter param2 = Parameter("3", CONSTANTS.FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWS, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.empty());
}

TEST_CASE("Checks that PKB pointer in WritePKB and ReadPKB is set to first pkb instance and not overwritten") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb1;
    FollowsStorage fs;
    pkb1.initializePkb();
    writePkb.setInstancePKB(pkb1);
    readPkb.setInstancePKB(pkb1);

    writePkb.setFollows(1, 2);

    PKB pkb2;
    FollowsStorage fs2;
    pkb2.initializePkb();
    writePkb.setInstancePKB(pkb2);

    Parameter param1 = Parameter("1", CONSTANTS.FIXED_INT);
    Parameter param2 = Parameter("2", CONSTANTS.FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWS, params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(unit_testing_utils::equals(check, res));

    readPkb.setInstancePKB(pkb2);

    res = readPkb.findRelationship(rs);
    REQUIRE(unit_testing_utils::equals(check, res));
}

