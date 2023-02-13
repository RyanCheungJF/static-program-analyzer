#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("Checks that write and read works for entityStorage") {
    EntityStorage es;
    Ent e = "v";
    std::vector<StmtNum> lines = {3, 6, 9};
    for (int line: lines) {
        es.writeEntity(e, line);
    }
    std::unordered_set<StmtNum> statementNums = es.getEntityStmtNums(e);

    bool res = true;
    res = res && statementNums.size() == 3;
    res = res && statementNums.find(3) != statementNums.end(); //3 is present
    res = res && statementNums.find(6) != statementNums.end(); //6 is present
    res = res && statementNums.find(9) != statementNums.end(); //9 is present
    res = res && statementNums.find(12) == statementNums.end(); //12 is not present
    REQUIRE(res);
}


TEST_CASE("Check that if an entity does not appear in the source code, it should return an empty set") {
    EntityStorage es;
    Ent e = "v";
    std::unordered_set<StmtNum> statementNums = es.getEntityStmtNums(e);

    bool res = true;
    res = res && statementNums.size() == 0;
    res = res && statementNums.find(3) == statementNums.end(); //3 is not present
    res = res && statementNums.find(12) == statementNums.end(); //12 is not present
    REQUIRE(res);
}

TEST_CASE("Check that given query for an entity and a statementNumber that it appears in, it returns true") {
    EntityStorage es;
    Ent e = "v";
    std::vector<StmtNum> lines = {3};
    for (int line: lines) {
        es.writeEntity(e, line);
    }

    bool res = true;
    res = res && (es.checkEntity(e, 3) == true);
    res = res && (es.checkEntity(e, 4) == false);
    REQUIRE(res);
}

TEST_CASE("Check that if an entity does not exist, it returns false") {
    EntityStorage es;
    bool res = true;
    res = res && (es.checkEntity("v", 4) == false);
    REQUIRE(res);
}

