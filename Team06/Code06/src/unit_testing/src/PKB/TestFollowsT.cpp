#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"
#include "../utils/utils.h"

using namespace std;
using namespace unit_testing_utils;

TEST_CASE("Check that all followers are recorded in the followee") {
    FollowsTStorage fts;

    std::unordered_set<StmtNum> followers = {2, 3, 4};
    fts.write(1, followers);

    REQUIRE(fts.exists(1, 2));
    REQUIRE(fts.exists(1, 3));
    REQUIRE(fts.exists(1, 4));
    REQUIRE(!fts.exists(1, 5));
}

TEST_CASE("Check that a follower is not recorded as a followee") {
    FollowsTStorage fts;

    std::unordered_set<StmtNum> followers = {2};
    fts.write(1, followers);

    REQUIRE(!fts.exists(1, 3));
}

TEST_CASE("Check that all of the followers of each followee is accurate, even with duplicate entries") {
    FollowsTStorage fts;
    std::unordered_set<StmtNum> followers_1 = {2, 3, 4, 4, 3, 4};
    std::unordered_set<StmtNum> followers_2 = {3, 4, 4, 3, 4};
    fts.write(1, followers_1);
    fts.write(2, followers_2);

    std::unordered_set<StmtNum> actual = fts.getRightWildcard(1);
    std::unordered_set<StmtNum> expected = {2, 4, 3};
    REQUIRE(unit_testing_utils::equals(expected, actual));

    std::unordered_set<StmtNum> actual2 = fts.getRightWildcard(2);
    std::unordered_set<StmtNum> expected2 = {4, 3};
    REQUIRE(unit_testing_utils::equals(expected2, actual2));
}

TEST_CASE("Check that all of the followees of each follower is accurate, even with duplicate entries") {
    FollowsTStorage fts;
    std::unordered_set<StmtNum> followers1 = {2, 3, 3, 4};
    std::unordered_set<StmtNum> followers2 = {3, 4};
    std::unordered_set<StmtNum> followers3 = {4};
    fts.write(1, followers1);
    fts.write(2, followers2);
    fts.write(3, followers3);

    std::unordered_set<StmtNum> actual1 = fts.getLeftWildcard(2);
    std::unordered_set<StmtNum> expected1 = { 1 };
    REQUIRE(unit_testing_utils::equals(expected1, actual1));

    std::unordered_set<StmtNum> actual2 = fts.getLeftWildcard(3);
    std::unordered_set<StmtNum> expected2 = { 1, 2 };
    REQUIRE(unit_testing_utils::equals(expected2, actual2));

    std::unordered_set<StmtNum> actual3 = fts.getLeftWildcard(4);
    std::unordered_set<StmtNum> expected3 = { 1, 2, 3};
    REQUIRE(unit_testing_utils::equals(expected3, actual3));
}


TEST_CASE("Checks for cases e.g. Follows*(1, 2)") {
    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> followers;
    followers.insert(2);
    writePkb.setFollowsT(1, followers);

    Parameter param1 = Parameter("1", CONSTANTS.FIXED_INT);
    Parameter param2 = Parameter("2", CONSTANTS.FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 1);
    REQUIRE(contains(res, {"1", "2"}));
}

TEST_CASE("Checks that a non-existent FollowsT relationship returns an empty vector from ReadPKB") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> followers;
    followers.insert(2);
    writePkb.setFollowsT(1, followers);

    Parameter param1 = Parameter("1", CONSTANTS.FIXED_INT);
    Parameter param2 = Parameter("3", CONSTANTS.FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.empty());
}

TEST_CASE("Checks for cases e.g. Follows*(1, assign)") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> followers;
    followers.insert(2);
    followers.insert(3);
    followers.insert(4);
    writePkb.setFollowsT(1, followers);
    writePkb.setStatement(CONSTANTS.ASSIGN, 2);
    writePkb.setStatement(CONSTANTS.ASSIGN, 3);
    writePkb.setStatement(CONSTANTS.IF, 4);

    Parameter param1 = Parameter("1", CONSTANTS.FIXED_INT);
    Parameter param2 = Parameter("a", CONSTANTS.ASSIGN);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 2);
    REQUIRE(contains(res, { "1", "2" }));
    REQUIRE(contains(res, { "1", "3" }));
}

TEST_CASE("Checks for cases e.g. Follows*(1, _)") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> followers;
    followers.insert(2);
    followers.insert(3);
    followers.insert(4);
    writePkb.setFollowsT(1, followers);
    writePkb.setStatement(CONSTANTS.ASSIGN, 2);
    writePkb.setStatement(CONSTANTS.ASSIGN, 3);
    writePkb.setStatement(CONSTANTS.IF, 4);

    Parameter param1 = Parameter("1", CONSTANTS.FIXED_INT);
    Parameter param2 = Parameter("_", CONSTANTS.WILDCARD);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 3);
    REQUIRE(contains(res, { "1", "2" }));
    REQUIRE(contains(res, { "1", "3" }));
    REQUIRE(contains(res, { "1", "4" }));
}

TEST_CASE("Checks for cases e.g. Follows*(if, 3)") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> followers;
    followers.insert(2);
    followers.insert(3);
    followers.insert(4);
    writePkb.setFollowsT(1, followers);
    writePkb.setStatement("while", 1);
    writePkb.setStatement("while", 2);

    Parameter param1 = Parameter("s", "stmt");
    Parameter param2 = Parameter("3", CONSTANTS.FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 1);
    REQUIRE(contains(res, { "1", "3" }));
}

TEST_CASE("Checks for cases e.g. Follows*(if, assign)") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> followers;
    followers.insert(2);
    followers.insert(3);
    followers.insert(4);
    writePkb.setFollowsT(1, followers);
    writePkb.setStatement("while", 1);
    writePkb.setStatement(CONSTANTS.IF, 2);
    writePkb.setStatement(CONSTANTS.IF, 3);

    Parameter param1 = Parameter("w", "while");
    Parameter param2 = Parameter(CONSTANTS.IF, CONSTANTS.IF);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 2);
    REQUIRE(contains(res, { "1", "2" }));
    REQUIRE(contains(res, { "1", "3" }));
}

TEST_CASE("Checks for cases e.g. Follows*(if, _)") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> followers1;
    followers1.insert(2);
    followers1.insert(3);
    followers1.insert(4);
    unordered_set<StmtNum> followers2;
    followers2.insert(3);
    writePkb.setFollowsT(1, followers1);
    writePkb.setFollowsT(2, followers2);
    writePkb.setStatement("while", 1);
    writePkb.setStatement("while", 2);

    Parameter param1 = Parameter("w", "while");
    Parameter param2 = Parameter("_", CONSTANTS.WILDCARD);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWST, params);


    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 4);
    REQUIRE(contains(res, { "1", "2" }));
    REQUIRE(contains(res, { "1", "3" }));
    REQUIRE(contains(res, { "1", "4" }));
    REQUIRE(contains(res, { "2", "3" }));
}


TEST_CASE("Checks for cases e.g. Follows*(_, 3)") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> followers1;
    followers1.insert(2);
    followers1.insert(3);
    unordered_set<StmtNum> followers2;
    followers2.insert(3);
    writePkb.setFollowsT(1, followers1);
    writePkb.setFollowsT(2, followers2);

    Parameter param1 = Parameter("_", CONSTANTS.WILDCARD);
    Parameter param2 = Parameter("3", CONSTANTS.FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 2);
    REQUIRE(contains(res, { "1", "3" }));
    REQUIRE(contains(res, { "2", "3" }));
}

TEST_CASE("Checks for cases e.g. Follows*(_, call)") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> followers1;
    followers1.insert(2);
    followers1.insert(3);
    unordered_set<StmtNum> followers2;
    followers2.insert(3);
    writePkb.setFollowsT(1, followers1);
    writePkb.setFollowsT(2, followers2);
    writePkb.setStatement(CONSTANTS.PRINT, 2);
    writePkb.setStatement(CONSTANTS.CALL, 3);

    Parameter param1 = Parameter("_", CONSTANTS.WILDCARD);
    Parameter param2 = Parameter("p", CONSTANTS.PRINT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 1);
    REQUIRE(contains(res, { "1", "2" }));
}

TEST_CASE("Checks for cases e.g. Follows*(_, _)") {

    AppConstants CONSTANTS;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> followers1;
    followers1.insert(2);
    followers1.insert(3);
    unordered_set<StmtNum> followers2;
    followers2.insert(3);
    writePkb.setFollowsT(1, followers1);
    writePkb.setFollowsT(2, followers2);

    Parameter param1 = Parameter("_", CONSTANTS.WILDCARD);
    Parameter param2 = Parameter("_", CONSTANTS.WILDCARD);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(CONSTANTS.FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 3);
    REQUIRE(contains(res, { "1", "2" }));
    REQUIRE(contains(res, { "1", "3" }));
    REQUIRE(contains(res, { "2", "3" }));
}

