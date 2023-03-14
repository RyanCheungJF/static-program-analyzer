#include "../../../spa/src/PKB/storage/EntityStorage.h"
#include "catch.hpp"

TEST_CASE("Checks that write and read works for entityStorage") {
    EntityStorage es;
    Ent e = "v";
    es.writeEntity(3, {e});
    es.writeEntity(6, {e});
    es.writeEntity(9, {e});
    std::unordered_set<StmtNum> statementNums = es.getEntityStmtNums(e);

    REQUIRE(statementNums.size() == 3);
    REQUIRE(statementNums.find(3) != statementNums.end());  // 3 is present
    REQUIRE(statementNums.find(6) != statementNums.end());  // 6 is present
    REQUIRE(statementNums.find(9) != statementNums.end());  // 9 is present
    REQUIRE(statementNums.find(12) == statementNums.end()); // 12 is not present
}

TEST_CASE("Check that if an entity does not appear in the source code, it should return an empty set") {
    EntityStorage es;
    Ent e = "v";
    std::unordered_set<StmtNum> statementNums = es.getEntityStmtNums(e);

    REQUIRE(statementNums.empty());
    REQUIRE(statementNums.find(3) == statementNums.end()); // 3 is not present
}

TEST_CASE("Check that given query for an entity and a statementNumber that it appears in, it returns true") {
    EntityStorage es;
    Ent e = "v";
    es.writeEntity(3, {e});

    REQUIRE(es.checkEntity(e, 3));
    REQUIRE(!es.checkEntity(e, 4));
}

TEST_CASE("Check that if an entity does not exist, it returns false") {
    EntityStorage es;
    REQUIRE(!es.checkEntity("v", 4));
}
