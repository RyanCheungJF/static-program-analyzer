#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("Checks that write and read works for constantStorage") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    ConstantStorage cs;
    pkb.constantStorage = &cs;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Const c = "5";
    writePkb.setConstant(c, 4);
    writePkb.setConstant(c, 8);
    writePkb.setConstant(c, 9);
    std::unordered_set<StmtNum> statementNums = readPkb.getConstantStatementNumbers(c);

    bool res = true;
    res = res && statementNums.size() == 3;
    res = res && statementNums.find(4) != statementNums.end(); //3 is present
    res = res && statementNums.find(8) != statementNums.end(); //6 is present
    res = res && statementNums.find(9) != statementNums.end(); //9 is present
    res = res && statementNums.find(12) == statementNums.end(); //12 is not present
    REQUIRE(res);
}

TEST_CASE("Check that if a constant does not appear in the source code, it should return an empty set") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    ConstantStorage cs;
    pkb.constantStorage = &cs;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Const c = "7";
    std::unordered_set<StmtNum> statementNums = readPkb.getConstantStatementNumbers(c);

    bool res = true;
    res = res && statementNums.size() == 0;
    res = res && statementNums.find(3) == statementNums.end(); //3 is not present
    res = res && statementNums.find(12) == statementNums.end(); //12 is not present
    REQUIRE(res);
}

TEST_CASE("Check that given query for a constant and a statementNumber that it appears in, it returns true") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    ConstantStorage cs;
    pkb.constantStorage = &cs;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Const c = "8";
    writePkb.setConstant(c, 3);

    bool res = true;
    res = res && (readPkb.checkConstant(c, 3) == true);
    res = res && (readPkb.checkConstant(c, 4) == false);
    REQUIRE(res);
}

TEST_CASE("Check that if a constant does not exist, it returns false") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    ConstantStorage cs;
    pkb.constantStorage = &cs;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    bool res = true;
    res = res && (readPkb.checkConstant("c", 4) == false);
    REQUIRE(res);
}

