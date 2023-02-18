#include "catch.hpp"
#include "../../../spa/src/PKB/storage/ParentStorage.h"

TEST_CASE("Check writes and reads to/from ParentStorage") {
    ParentStorage par;

    par.write(1, 2);
    bool res = par.exists(1, 2);
    REQUIRE(res);

    par.write(1, 2);
    res = par.exists(1, 3);
    REQUIRE(!res);

    par.write(1, 2);
    res = par.exists(3, 2);
    REQUIRE(!res);
}

TEST_CASE("Tests for getting children") {
    ParentStorage par;

    par.write(1, 2);
    par.write(1, 3);

    std::unordered_set<StmtNum> res = par.getRightWildcard(1);
    std::unordered_set<StmtNum> check{ 2, 3 };
    REQUIRE(res == check);

    res = par.getRightWildcard(2);
    check = {};
    REQUIRE(res == check);

    res = par.getRightWildcard(3);
    check = {};
    REQUIRE(res == check);
}

TEST_CASE("Tests for getting parent") {
    ParentStorage par;

    par.write(1, 2);

    std::unordered_set<StmtNum> res = par.getLeftWildcard(2);
    std::unordered_set<StmtNum> check{ 1 };
    REQUIRE(res == check);

    res = par.getLeftWildcard(1);
    check = {};
    REQUIRE(res == check);

    res = par.getLeftWildcard(3);
    check = {};
    REQUIRE(res == check);
}
