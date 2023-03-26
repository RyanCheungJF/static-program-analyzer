#include "../../../spa/src/PKB/storage/EntityStorage.h"
#include "catch.hpp"

TEST_CASE("Checks that write and read works for entityStorage") {
    EntityStorage<Ent> es;
    Ent e = "v";
    std::unordered_set<Ent> vals = {e};
    es.writeEntity(3, vals);
    es.writeEntity(6, vals);
    es.writeEntity(9, vals);
    std::unordered_set<StmtNum> statementNums = es.getEntityStmtNums(e);

    REQUIRE(statementNums.size() == 3);
    REQUIRE(statementNums.find(3) != statementNums.end());  // 3 is present
    REQUIRE(statementNums.find(6) != statementNums.end());  // 6 is present
    REQUIRE(statementNums.find(9) != statementNums.end());  // 9 is present
    REQUIRE(statementNums.find(12) == statementNums.end()); // 12 is not present
}

TEST_CASE("Check that if an entity does not appear in the source code, it should return an empty set") {
    EntityStorage<Ent> es;
    Ent e = "v";
    std::unordered_set<StmtNum> statementNums = es.getEntityStmtNums(e);

    REQUIRE(statementNums.empty());
    REQUIRE(statementNums.find(3) == statementNums.end()); // 3 is not present
}

TEST_CASE("Check that given query for an entity and a statementNumber that it appears in, it returns true") {
    EntityStorage<Ent> es;
    Ent e = "v";
    std::unordered_set<Ent> vals = {e};
    es.writeEntity(3, vals);

    REQUIRE(es.checkEntity(e, 3));
    REQUIRE(!es.checkEntity(e, 4));
}

TEST_CASE("Check that if an entity does not exist, it returns false") {
    EntityStorage<Ent> es;
    REQUIRE(!es.checkEntity("v", 4));
}
