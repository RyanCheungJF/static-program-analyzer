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
