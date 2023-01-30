#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("Check that all followers are recorded in the followee") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({1, 2});
    followee_follower.push_back({1, 3});
    followee_follower.push_back({1, 4});
    writePkb.setFollowsT(followee_follower);

    bool res = true;
    for (std::pair<StmtNum, StmtNum> p: followee_follower) {
        res = res && readPkb.checkFollowsT(p.first, p.second);
    }
    REQUIRE(res);
}

TEST_CASE("Check that a follower is not recorded as a followee") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({1, 2});
    writePkb.setFollowsT(followee_follower);

    bool res = readPkb.checkFollowsT(1, 3);
    REQUIRE(res == false);
}

TEST_CASE("Check that all of the followers of each followee is accurate, even with duplicate entries") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({1, 2});
    followee_follower.push_back({1, 3});
    followee_follower.push_back({1, 4});
    followee_follower.push_back({1, 4});
    followee_follower.push_back({1, 4});
    followee_follower.push_back({2, 3});
    followee_follower.push_back({2, 4});
    followee_follower.push_back({2, 4});
    writePkb.setFollowsT(followee_follower);

    bool res = true;
    std::unordered_set<StmtNum> followers1 = readPkb.getFollowersT(1);
    res = res && followers1.size() == 3;
    res = res && followers1.find(2) != followers1.end();
    res = res && followers1.find(3) != followers1.end();
    res = res && followers1.find(4) != followers1.end();

    std::unordered_set<StmtNum> followers2 = readPkb.getFollowersT(2);
    res = res && followers2.size() == 2;
    res = res && followers2.find(3) != followers2.end();
    res = res && followers2.find(4) != followers2.end();

    REQUIRE(res);
}

TEST_CASE("Check that all of the followees of each follower is accurate, even with duplicate entries") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({1, 2});
    followee_follower.push_back({1, 3});
    followee_follower.push_back({1, 4});
    followee_follower.push_back({1, 4});
    followee_follower.push_back({1, 4});
    followee_follower.push_back({2, 3});
    followee_follower.push_back({2, 4});
    followee_follower.push_back({2, 4});
    followee_follower.push_back({3, 4});
    followee_follower.push_back({3, 4});
    writePkb.setFollowsT(followee_follower);

    bool res = true;
    std::unordered_set<StmtNum> followees2 = readPkb.getFolloweesT(2);
    res = res && followees2.size() == 1;
    res = res && followees2.find(1) != followees2.end();

    std::unordered_set<StmtNum> followees3 = readPkb.getFolloweesT(3);
    res = res && followees3.size() == 2;
    res = res && followees3.find(1) != followees3.end();
    res = res && followees3.find(2) != followees3.end();

    std::unordered_set<StmtNum> followees4 = readPkb.getFolloweesT(4);
    res = res && followees4.size() == 3;
    res = res && followees4.find(1) != followees4.end();
    res = res && followees4.find(2) != followees4.end();
    res = res && followees4.find(3) != followees4.end();

    REQUIRE(res);
}

//TODO: Probably needs to be a standalone component itself - where the component can understand which tables to join.
// I'm just doing it here to test out set-wise operation logic
// also need to do some test queries for Variables/Constants table, Procedures Table
/**
 * Given a PQL query of `if i; Select i such that Follows*(1, i)`
 * We want to find all if-statements that follows* line 1
 */
TEST_CASE("Check that we can find an if statement from a Follows relationship") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;

    // line 2 is the if-statement and line 4 is the last line in the else clause
    // line 5 is a normal statement nested on the same level as line 1
    followee_follower.push_back({1, 2});
    followee_follower.push_back({1, 5});

    // line 6 is the if-statement and line 9 is the last line in the else clause
    // line 10 is a normal statement nested on the same level as line 1
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
    /*
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
    */
}

