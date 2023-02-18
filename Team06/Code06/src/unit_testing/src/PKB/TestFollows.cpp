#include "catch.hpp"
#include "../../../spa/src/PKB/storage/FollowsStorage.h"

TEST_CASE("FollowsStorage: write(StmtNum followee, StmtNum follower)") {
    FollowsStorage fs;

    SECTION("exists(StmtNum followee, StmtNum follower): empty storage") {
        REQUIRE(!fs.exists(1, 2));
    }

    SECTION("getRightWildcard(StmtNum leftStmtNum): empty storage") {
        std::unordered_set<StmtNum> res = fs.getRightWildcard(1);
        REQUIRE(res.size() == 0);
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
        std::unordered_set<StmtNum> res = fs.getRightWildcard(1);
        std::vector<StmtNum> actual;
        for (auto i : res) {
            actual.push_back(i);
        }
        std::vector<StmtNum> expected = { 2 };
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(res.size() == 1);
    }

    SECTION("getLeftWildcard(StmtNum leftStmtNum): non-empty storage") {
        std::unordered_set<StmtNum> res = fs.getLeftWildcard(2);
        std::vector<StmtNum> actual;
        for (auto i : res) {
            actual.push_back(i);
        }
        std::vector<StmtNum> expected = { 1 };
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(res.size() == 1);
    }

}

TEST_CASE("Checks FollowsStorage such that given a followee, if it does not have a certain follower, an empty vector is returned") {

    FollowsStorage fs;

    fs.write(1, 2);
    std::unordered_set<StmtNum> res = fs.getRightWildcard(3);
    std::unordered_set<StmtNum> check;
    REQUIRE(res == check);
}

TEST_CASE("Checks FollowsStorage such that given a follower, if it does not have a certain followee, an empty vector is returned") {

    FollowsStorage fs;

    fs.write(1, 2);
    std::unordered_set<StmtNum> res = fs.getLeftWildcard(3);
    std::unordered_set<StmtNum> check;
    REQUIRE(res == check);
}

