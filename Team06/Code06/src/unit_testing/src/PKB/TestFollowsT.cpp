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

// Test cases should be done. Waiting on Relationship object to support FollowsT Relationship
/*
TEST_CASE("Checks for cases e.g. Follows*(1, 2)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsTStorage fts;
    pkb.followsTStorage = &fts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({ 1, 2 });
    writePkb.setFollowsT(followee_follower);


    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("2", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks that a non-existent FollowsT relationship returns an empty vector from ReadPKB") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsTStorage fts;
    pkb.followsTStorage = &fts;
    StmtStorage sts;
    pkb.statementStorage = &sts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({ 1, 2 });
    writePkb.setFollowsT(followee_follower);

    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("3", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check;
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks for cases e.g. Follows*(1, assign)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsTStorage fts;
    pkb.followsTStorage = &fts;
    StmtStorage sts;
    pkb.statementStorage = &sts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({ 1, 2 });
    followee_follower.push_back({ 1, 3 });
    followee_follower.push_back({ 1, 4 });
    writePkb.setFollowsT(followee_follower);
    writePkb.setStatement("assign", 2);
    writePkb.setStatement("assign", 3);
    writePkb.setStatement("if", 4);

    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("a", "assign");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"}, {"1", "3"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks for cases e.g. Follows*(1, _)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsTStorage fts;
    pkb.followsTStorage = &fts;
    StmtStorage sts;
    pkb.statementStorage = &sts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({ 1, 2 });
    followee_follower.push_back({ 1, 3 });
    followee_follower.push_back({ 1, 4 });
    writePkb.setFollowsT(followee_follower);
    writePkb.setStatement("assign", 2);
    writePkb.setStatement("assign", 3);
    writePkb.setStatement("if", 4);

    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("_", "wildcard");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"}, {"1", "3"}, {"1", "4"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks for cases e.g. Follows*(if, 3)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsTStorage fts;
    pkb.followsTStorage = &fts;
    StmtStorage sts;
    pkb.statementStorage = &sts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({ 1, 2 });
    followee_follower.push_back({ 1, 3 });
    followee_follower.push_back({ 1, 4 });
    writePkb.setFollowsT(followee_follower);
    writePkb.setStatement("while", 1);
    writePkb.setStatement("while", 2);

    Parameter param1 = Parameter("s", "stmt");
    Parameter param2 = Parameter("3", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check = { {"1", "3"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks for cases e.g. Follows*(if, assign)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsTStorage fs;
    pkb.followsTStorage = &fs;
    StmtStorage sts;
    pkb.statementStorage = &sts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({ 1, 2 });
    followee_follower.push_back({ 1, 3 });
    followee_follower.push_back({ 1, 4 });
    writePkb.setFollowsT(followee_follower);
    writePkb.setStatement("while", 1);
    writePkb.setStatement("if", 2);
    writePkb.setStatement("if", 3);

    Parameter param1 = Parameter("w", "while");
    Parameter param2 = Parameter("if", "if");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"}, {"1", "3"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks for cases e.g. Follows*(if, _)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsTStorage fts;
    pkb.followsTStorage = &fts;
    StmtStorage sts;
    pkb.statementStorage = &sts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({ 1, 2 });
    followee_follower.push_back({ 2, 3 });
    followee_follower.push_back({ 1, 3 });
    followee_follower.push_back({ 1, 4 });
    writePkb.setFollowsT(followee_follower);
    writePkb.setStatement("while", 1);
    writePkb.setStatement("while", 2);

    Parameter param1 = Parameter("w", "while");
    Parameter param2 = Parameter("_", "wildcard");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows*", params);


    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    std::unordered_set< std::vector<std::string>> check;
    for (auto pair : res) {
        check.insert(pair);
    }
    REQUIRE(check.size() == 4);
}


TEST_CASE("Checks for cases e.g. Follows*(_, 3)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsTStorage fts;
    pkb.followsTStorage = &fts;
    StmtStorage sts;
    pkb.statementStorage = &sts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({ 1, 2 });
    followee_follower.push_back({ 2, 3 });
    followee_follower.push_back({ 1, 3 });
    writePkb.setFollowsT(followee_follower);

    Parameter param1 = Parameter("_", "wildcard");
    Parameter param2 = Parameter("3", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    std::unordered_set< std::vector<std::string>> check;
    for (auto pair : res) {
        check.insert(pair);
    }
    REQUIRE(check.size() == 2);
}

TEST_CASE("Checks for cases e.g. Follows*(_, call)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsTStorage fts;
    pkb.followsTStorage = &fts;
    StmtStorage sts;
    pkb.statementStorage = &sts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({ 1, 2 });
    followee_follower.push_back({ 2, 3 });
    followee_follower.push_back({ 1, 3 });
    writePkb.setFollowsT(followee_follower);
    writePkb.setStatement("print", 2);
    writePkb.setStatement("call", 3);

    Parameter param1 = Parameter("_", "wildcard");
    Parameter param2 = Parameter("p", "print");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks for cases e.g. Follows*(_, _)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsTStorage fts;
    pkb.followsTStorage = &fts;
    StmtStorage sts;
    pkb.statementStorage = &sts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({ 1, 2 });
    followee_follower.push_back({ 2, 3 });
    followee_follower.push_back({ 1, 3 });
    writePkb.setFollowsT(followee_follower);

    Parameter param1 = Parameter("_", "wildcard");
    Parameter param2 = Parameter("_", "wildcard");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows*", params);

    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    std::unordered_set< std::vector<std::string>> check;
    for (auto pair : res) {
        check.insert(pair);
    }
    REQUIRE(check.size() == 3);
}
*/

//TODO: Probably needs to be a standalone component itself - where the component can understand which tables to join.
// I'm just doing it here to test out set-wise operation logic
// also need to do some test queries for Variables/Constants table, Procedures Table
/**
 * Given a PQL query of `if i; Select i such that Follows*(1, i)`
 * We want to find all if-statements that follows* line 1
 */

/*TEST_CASE("Check that we can find an if statement from a Follows relationship") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsT ft;
    StatementAPI statement;
    pkb.followsTApi = &ft;
    pkb.statementApi = &statement;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;

    // line 2 is the if-StatementAPI and line 4 is the last line in the else clause
    // line 5 is a normal StatementAPI nested on the same level as line 1
    followee_follower.push_back({1, 2});
    followee_follower.push_back({1, 5});

    // line 6 is the if-StatementAPI and line 9 is the last line in the else clause
    // line 10 is a normal StatementAPI nested on the same level as line 1
    followee_follower.push_back({1, 6});
    followee_follower.push_back({1, 10});
    writePkb.setFollowsT(followee_follower);

    Stmt s = "if";
    std::vector<StmtNum> lines;
    lines.push_back(2);
    lines.push_back(6);
    writePkb.setStatement(s, lines);


    std::unordered_set<StmtNum> followersStatementNums = readPkb.getFollowersT(1);
    std::unordered_set<StmtNum> ifStatementNums = readPkb.getStatementNumbers(s);

    std::unordered_set<StmtNum> smaller;
    std::unordered_set<StmtNum> larger;
    if (ifStatementNums.size() > followersStatementNums.size()) {
        smaller = followersStatementNums;
        larger = ifStatementNums;
    } else {
        smaller = ifStatementNums;
        larger = followersStatementNums;
    }

    std::vector<StmtNum> commonStatementNums;
    for (StmtNum n: smaller) {
        if (larger.find(n) != larger.end()) {
            commonStatementNums.push_back(n);
        }
    }

    std::sort(commonStatementNums.begin(), commonStatementNums.end());
    vector<StmtNum> correct{2, 6};

    bool res = std::equal(commonStatementNums.begin(), commonStatementNums.begin(), correct.begin());
    REQUIRE(res);


    // prints out followersStatementNums
    std::copy(followersStatementNums.begin(),
              followersStatementNums.end(),
              std::ostream_iterator<int>(std::cout, " "));

    std::cout << "\n";

    // prints out ifStatementNums
    std::copy(ifStatementNums.begin(),
              ifStatementNums.end(),
              std::ostream_iterator<int>(std::cout, " "));

    // prints out commonStatementNums
    std::copy(commonStatementNums.begin(),
              commonStatementNums.end(),
              std::ostream_iterator<int>(std::cout, " "));

}
*/
