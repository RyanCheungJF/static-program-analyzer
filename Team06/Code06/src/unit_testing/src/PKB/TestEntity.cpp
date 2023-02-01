#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("Checks that write and read works for entityStorage") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    EntityStorage es;
    pkb.entityStorage = &es;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Ent e = "v";
    std::vector<StmtNum> lines;
    lines.push_back(3);
    lines.push_back(6);
    lines.push_back(9);
    writePkb.setEntity(e, lines);
    std::unordered_set<StmtNum> statementNums = readPkb.getEntityStatementNumbers(e);

    bool res = true;
    res = res && statementNums.size() == 3;
    res = res && statementNums.find(3) != statementNums.end(); //3 is present
    res = res && statementNums.find(6) != statementNums.end(); //6 is present
    res = res && statementNums.find(9) != statementNums.end(); //9 is present
    res = res && statementNums.find(12) == statementNums.end(); //12 is not present
    REQUIRE(res);
}

//TODO: similar test cases to statement
