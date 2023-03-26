#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace std;
using namespace unit_testing_utils;

TEST_CASE("Check that all followers are recorded in the followee") {
    RelationshipStorage<StmtNum, StmtNum> fts;

    std::unordered_set<StmtNum> followers = {2, 3, 4};
    fts.write(1, followers);

    REQUIRE(fts.exists(1, 2));
    REQUIRE(fts.exists(1, 3));
    REQUIRE(fts.exists(1, 4));
    REQUIRE(!fts.exists(1, 5));
}

TEST_CASE("Check that a follower is not recorded as a followee") {
    RelationshipStorage<StmtNum, StmtNum> fts;

    std::unordered_set<StmtNum> followers = {2};
    fts.write(1, followers);

    REQUIRE(!fts.exists(1, 3));
}

TEST_CASE("Check that all of the followers of each followee is accurate, even with duplicate entries") {
    RelationshipStorage<StmtNum, StmtNum> fts;
    std::unordered_set<StmtNum> followers_1 = {2, 3, 4, 4, 3, 4};
    std::unordered_set<StmtNum> followers_2 = {3, 4, 4, 3, 4};
    fts.write(1, followers_1);
    fts.write(2, followers_2);

    std::unordered_set<StmtNum> actual = *fts.getRightItems(1);
    std::unordered_set<StmtNum> expected = {2, 4, 3};
    REQUIRE(unit_testing_utils::equals(expected, actual));

    std::unordered_set<StmtNum> actual2 = *fts.getRightItems(2);
    std::unordered_set<StmtNum> expected2 = {4, 3};
    REQUIRE(unit_testing_utils::equals(expected2, actual2));
}

TEST_CASE("Check that all of the followees of each follower is accurate, even with duplicate entries") {
    RelationshipStorage<StmtNum, StmtNum> fts;
    std::unordered_set<StmtNum> followers1 = {2, 3, 3, 4};
    std::unordered_set<StmtNum> followers2 = {3, 4};
    std::unordered_set<StmtNum> followers3 = {4};
    fts.write(1, followers1);
    fts.write(2, followers2);
    fts.write(3, followers3);

    std::unordered_set<StmtNum> actual1 = *fts.getLeftItems(2);
    std::unordered_set<StmtNum> expected1 = {1};
    REQUIRE(unit_testing_utils::equals(expected1, actual1));

    std::unordered_set<StmtNum> actual2 = *fts.getLeftItems(3);
    std::unordered_set<StmtNum> expected2 = {1, 2};
    REQUIRE(unit_testing_utils::equals(expected2, actual2));

    std::unordered_set<StmtNum> actual3 = *fts.getLeftItems(4);
    std::unordered_set<StmtNum> expected3 = {1, 2, 3};
    REQUIRE(unit_testing_utils::equals(expected3, actual3));
}

TEST_CASE("Checks for cases e.g. Follows*(1, 2)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> followers;
    followers.insert(2);
    writePkb.setFollowsT(1, followers);

    Parameter param1 = Parameter("1", ParameterType::FIXED_INT);
    Parameter param2 = Parameter("2", ParameterType::FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 1);
    REQUIRE(contains(res, {"1", "2"}));
}

TEST_CASE("Checks that a non-existent FollowsT relationship returns an empty vector from ReadPKB") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    unordered_set<StmtNum> followers;
    followers.insert(2);
    writePkb.setFollowsT(1, followers);

    Parameter param1 = Parameter("1", ParameterType::FIXED_INT);
    Parameter param2 = Parameter("3", ParameterType::FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.empty());
}

TEST_CASE("Checks for cases e.g. Follows*(1, assign)") {

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
    writePkb.setStatement(AppConstants::ASSIGN, 2);
    writePkb.setStatement(AppConstants::ASSIGN, 3);
    writePkb.setStatement(AppConstants::IF, 4);

    Parameter param1 = Parameter("1", ParameterType::FIXED_INT);
    Parameter param2 = Parameter("a", ParameterType::ASSIGN);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 2);
    REQUIRE(contains(res, {"1", "2"}));
    REQUIRE(contains(res, {"1", "3"}));
}

TEST_CASE("Checks for cases e.g. Follows*(1, _)") {

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
    writePkb.setStatement(AppConstants::ASSIGN, 2);
    writePkb.setStatement(AppConstants::ASSIGN, 3);
    writePkb.setStatement(AppConstants::IF, 4);

    Parameter param1 = Parameter("1", ParameterType::FIXED_INT);
    Parameter param2 = Parameter("_", ParameterType::WILDCARD);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 3);
    REQUIRE(contains(res, {"1", "2"}));
    REQUIRE(contains(res, {"1", "3"}));
    REQUIRE(contains(res, {"1", "4"}));
}

TEST_CASE("Checks for cases e.g. Follows*(if, 3)") {

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
    writePkb.setStatement(AppConstants::WHILE, 1);
    writePkb.setStatement(AppConstants::WHILE, 2);

    Parameter param1 = Parameter("s", ParameterType::STMT);
    Parameter param2 = Parameter("3", ParameterType::FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 1);
    REQUIRE(contains(res, {"1", "3"}));
}

TEST_CASE("Checks for cases e.g. Follows*(if, assign)") {

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
    writePkb.setStatement(AppConstants::WHILE, 1);
    writePkb.setStatement(AppConstants::IF, 2);
    writePkb.setStatement(AppConstants::IF, 3);

    Parameter param1 = Parameter("w", ParameterType::WHILE);
    Parameter param2 = Parameter("if", ParameterType::IF);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 2);
    REQUIRE(contains(res, {"1", "2"}));
    REQUIRE(contains(res, {"1", "3"}));
}

TEST_CASE("Checks for cases e.g. Follows*(if, _)") {

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
    writePkb.setStatement(AppConstants::WHILE, 1);
    writePkb.setStatement(AppConstants::WHILE, 2);

    Parameter param1 = Parameter("w", ParameterType::WHILE);
    Parameter param2 = Parameter("_", ParameterType::WILDCARD);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 4);
    REQUIRE(contains(res, {"1", "2"}));
    REQUIRE(contains(res, {"1", "3"}));
    REQUIRE(contains(res, {"1", "4"}));
    REQUIRE(contains(res, {"2", "3"}));
}

TEST_CASE("Checks for cases e.g. Follows*(_, 3)") {

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

    Parameter param1 = Parameter("_", ParameterType::WILDCARD);
    Parameter param2 = Parameter("3", ParameterType::FIXED_INT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 2);
    REQUIRE(contains(res, {"1", "3"}));
    REQUIRE(contains(res, {"2", "3"}));
}

TEST_CASE("Checks for cases e.g. Follows*(_, call)") {

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
    writePkb.setStatement(AppConstants::PRINT, 2);
    writePkb.setStatement(AppConstants::CALL, 3);

    Parameter param1 = Parameter("_", ParameterType::WILDCARD);
    Parameter param2 = Parameter("p", ParameterType::PRINT);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 1);
    REQUIRE(contains(res, {"1", "2"}));
}

TEST_CASE("Checks for cases e.g. Follows*(_, _)") {

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

    Parameter param1 = Parameter("_", ParameterType::WILDCARD);
    Parameter param2 = Parameter("_", ParameterType::WILDCARD);
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship(AppConstants::FOLLOWST, params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(res.size() == 3);
    REQUIRE(contains(res, {"1", "2"}));
    REQUIRE(contains(res, {"1", "3"}));
    REQUIRE(contains(res, {"2", "3"}));
}
