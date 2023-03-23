#include "../../../spa/src/PKB/storage/EntityStorage.h"
#include "catch.hpp"

TEST_CASE("Checks that write and read works for constantStorage") {
    EntityStorage<Const> cs;
    Const c = "5";
    cs.writeEntity(4, {c, "10"});
    cs.writeEntity(8, {c});
    cs.writeEntity(9, {c});
    std::unordered_set<StmtNum> statementNums = cs.getEntityStmtNums(c);

    REQUIRE(statementNums.size() == 3);
    REQUIRE(statementNums.find(4) != statementNums.end());  // 4 is present
    REQUIRE(statementNums.find(8) != statementNums.end());  // 8 is present
    REQUIRE(statementNums.find(9) != statementNums.end());  // 9 is present
    REQUIRE(statementNums.find(12) == statementNums.end()); // 12 is not present
}

TEST_CASE("Check that if a constant does not appear in the source code, it should return an empty set") {
    EntityStorage<Const> cs;
    Const c = "7";
    std::unordered_set<StmtNum> statementNums = cs.getEntityStmtNums(c);

    REQUIRE(statementNums.empty());
    REQUIRE(statementNums.find(3) == statementNums.end()); // 3 is not present
}

TEST_CASE("Check that given query for a constant and a statementNumber that it appears in, it returns true") {
    EntityStorage<Const> cs;
    Const c = "8";
    std::unordered_set<Const> set = {c, "10", "13"};
    cs.writeEntity(3, set);

    REQUIRE(cs.checkEntity(c, 3));
    REQUIRE(!cs.checkEntity(c, 4));
}

TEST_CASE("Check that if a constant does not exist, it returns false") {
    EntityStorage<Const> cs;
    REQUIRE(!cs.checkEntity("3", 4));
}
