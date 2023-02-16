#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("Check that all followers are recorded in the followee") {
    FollowsTStorage fts;

    std::unordered_set<StmtNum> followers = {2, 3, 4};
    fts.write(1, followers);

    bool res = true;
    res = res && fts.exists(1, 2) && fts.exists(1, 3) && fts.exists(1, 4);
    res = res && !fts.exists(1, 5);
    REQUIRE(res);
}

TEST_CASE("Check that a follower is not recorded as a followee") {
    FollowsTStorage fts;

    std::unordered_set<StmtNum> followers = {2};
    fts.write(1, followers);

    bool res = fts.exists(1, 3);
    REQUIRE(res == false);
}

TEST_CASE("Check that all of the followers of each followee is accurate, even with duplicate entries") {
    FollowsTStorage fts;
    std::unordered_set<StmtNum> followers_1 = {2, 3, 4, 4, 3, 4};
    std::unordered_set<StmtNum> followers_2 = {3, 4, 4, 3, 4};
    fts.write(1, followers_1);
    fts.write(2, followers_2);

    bool res = true;
    std::unordered_set<StmtNum> followers1 = fts.getRightWildcard(1);
    res = res && followers1.size() == 3;
    res = res && followers1.find(2) != followers1.end();
    res = res && followers1.find(3) != followers1.end();
    res = res && followers1.find(4) != followers1.end();

    REQUIRE(res);

    std::unordered_set<StmtNum> followers2 = fts.getRightWildcard(2);
    res = res && followers2.size() == 2;
    res = res && followers2.find(3) != followers2.end();
    res = res && followers2.find(4) != followers2.end();

    REQUIRE(res);
}

TEST_CASE("Check that all of the followees of each follower is accurate, even with duplicate entries") {
    FollowsTStorage fts;
    std::unordered_set<StmtNum> followers1 = {2, 3, 3, 4};
    std::unordered_set<StmtNum> followers2 = {3, 4};
    std::unordered_set<StmtNum> followers3 = {4};
    fts.write(1, followers1);
    fts.write(2, followers2);
    fts.write(3, followers3);

    bool res = true;
    std::unordered_set<StmtNum> followees2 = fts.getLeftWildcard(2);
    res = res && followees2.size() == 1;
    res = res && followees2.find(1) != followees2.end();

    REQUIRE(res);

    std::unordered_set<StmtNum> followees3 = fts.getLeftWildcard(3);
    res = res && followees3.size() == 2;
    res = res && followees3.find(1) != followees3.end();
    res = res && followees3.find(2) != followees3.end();

    REQUIRE(res);

    std::unordered_set<StmtNum> followees4 = fts.getLeftWildcard(4);
    res = res && followees4.size() == 3;
    res = res && followees4.find(1) != followees4.end();
    res = res && followees4.find(2) != followees4.end();
    res = res && followees4.find(3) != followees4.end();

    REQUIRE(res);
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

    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("2", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
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

    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("3", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check;
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
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
    writePkb.setStatement("assign", 2);
    writePkb.setStatement("assign", 3);
    writePkb.setStatement("if", 4);

    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("a", "assign");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"}, {"1", "3"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
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
    writePkb.setStatement("assign", 2);
    writePkb.setStatement("assign", 3);
    writePkb.setStatement("if", 4);

    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("_", "wildcard");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"}, {"1", "3"}, {"1", "4"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
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
    writePkb.setStatement("while", 1);
    writePkb.setStatement("while", 2);

    Parameter param1 = Parameter("s", "stmt");
    Parameter param2 = Parameter("3", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check = { {"1", "3"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
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
    writePkb.setStatement("while", 1);
    writePkb.setStatement("if", 2);
    writePkb.setStatement("if", 3);

    Parameter param1 = Parameter("w", "while");
    Parameter param2 = Parameter("if", "if");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"}, {"1", "3"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
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
    writePkb.setStatement("while", 1);
    writePkb.setStatement("while", 2);

    Parameter param1 = Parameter("w", "while");
    Parameter param2 = Parameter("_", "wildcard");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Follows*", params);


    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    std::vector<std::vector<std::string>> check = { { "1", "2" }, { "1", "3" }, { "1", "4" }, { "2", "3" } };
    REQUIRE(res == check);
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

    Parameter param1 = Parameter("_", "wildcard");
    Parameter param2 = Parameter("3", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    std::vector<std::vector<std::string>> check = { { "1", "3" }, { "2", "3" } };
    REQUIRE(check == res);
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
    writePkb.setStatement("print", 2);
    writePkb.setStatement("call", 3);

    Parameter param1 = Parameter("_", "wildcard");
    Parameter param2 = Parameter("p", "print");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
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

    Parameter param1 = Parameter("_", "wildcard");
    Parameter param2 = Parameter("_", "wildcard");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    shared_ptr<Relationship> rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    std::vector<std::vector<std::string>> check = { { "1", "2" }, { "1", "3" }, { "2", "3" } };
    REQUIRE(res == check);
}

