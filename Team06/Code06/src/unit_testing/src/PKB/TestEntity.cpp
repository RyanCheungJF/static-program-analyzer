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

TEST_CASE("Check that if an entity does not appear in the source code, it should return an empty set") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    EntityStorage et;
    pkb.entityStorage = &et;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Ent e = "v";
    std::vector<StmtNum> lines;
    writePkb.setEntity(e, lines);
    std::unordered_set<StmtNum> statementNums = readPkb.getEntityStatementNumbers(e);

    bool res = true;
    res = res && statementNums.size() == 0;
    res = res && statementNums.find(3) == statementNums.end(); //3 is not present
    res = res && statementNums.find(12) == statementNums.end(); //12 is not present
    REQUIRE(res);
}

TEST_CASE("Check that given query for an entity and a statementNumber that it appears in, it returns true") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    EntityStorage et;
    pkb.entityStorage = &et;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Ent e = "v";
    std::vector<StmtNum> lines;
    lines.push_back(3);
    writePkb.setEntity(e, lines);

    bool res = true;
    res = res && (readPkb.checkEntity(e, 3) == true);
    res = res && (readPkb.checkEntity(e, 4) == false);
    REQUIRE(res);
}

TEST_CASE("Check that if an entity does not exist, it returns false") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    EntityStorage et;
    pkb.entityStorage = &et;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    bool res = true;
    res = res && (readPkb.checkEntity("v", 4) == false);
    REQUIRE(res);
}

TEST_CASE("Check that given an entity and their statement numbers, a query for an non-existent statement returns false") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Statement st;
    pkb.statementApi = &st;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    std::vector<StmtNum> lines;
    lines.push_back(3);
    writePkb.setStatement(s, lines);

    REQUIRE(readPkb.checkStatement("while", 200) == false);
}


