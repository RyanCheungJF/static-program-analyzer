#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

TEST_CASE("Check writes and reads to/from ParentTStorage") {
    ParentTStorage pts;
    std::unordered_set<StmtNum> children = {2, 3, 4};
    pts.write(1, children);

    bool res = pts.exists(1, 2);
    REQUIRE(res);

    res = pts.exists(1, 4);
    REQUIRE(res);

    res = pts.exists(1, 5);
    REQUIRE(!res);

    res = pts.exists(3, 2);
    REQUIRE(!res);
}

TEST_CASE("Tests for getting children for ParentTStorage") {
    ParentTStorage pts;
    std::unordered_set<StmtNum> children = {2, 3, 4};
    pts.write(1, children);

    std::unordered_set<StmtNum> res = pts.getRightWildcard(1);
    std::unordered_set<StmtNum> check{ 2, 3, 4 };
    REQUIRE(res == check);

    res = pts.getRightWildcard(2);
    check = {};
    REQUIRE(res == check);

    res = pts.getRightWildcard(3);
    check = {};
    REQUIRE(res == check);
}

TEST_CASE("Tests for getting parent for ParentTStorage") {
    ParentTStorage pts;
    std::unordered_set<StmtNum> children1 = {2, 3, 4};
    std::unordered_set<StmtNum> children2 = {3};
    pts.write(1, children1);
    pts.write(2, children2);

    std::unordered_set<StmtNum> res = pts.getLeftWildcard(3);
    std::unordered_set<StmtNum> check{ 1, 2 };
    REQUIRE(res == check);

    res = pts.getLeftWildcard(1);
    check = {};
    REQUIRE(res == check);

    res = pts.getLeftWildcard(5);
    check = {};
    REQUIRE(res == check);
}
