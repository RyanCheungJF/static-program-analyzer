#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("All statement numbers are recorded in their respective procedures") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Proc p = "calculateEuclidean";
    std::vector<StmtNum> lines;
    lines.push_back(2);
    lines.push_back(3);
    lines.push_back(4);
    writePkb.setProcedure(p, lines);
    std::unordered_set<StmtNum> statementNums = readPkb.getProcedureStatementNumbers(p);

    bool res = true;
    res = res && statementNums.size() == 3;
    res = res && statementNums.find(2) != statementNums.end(); //3 is present
    res = res && statementNums.find(3) != statementNums.end(); //6 is present
    res = res && statementNums.find(4) != statementNums.end(); //9 is present
    res = res && statementNums.find(12) == statementNums.end(); //12 is not present
    REQUIRE(res);
}

TEST_CASE("If a procedure does not appear in the source code, getProcedureStatementNumbers() should return an empty set") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Proc p = "calculateEuclidean";
    std::unordered_set<StmtNum> statementNums = readPkb.getProcedureStatementNumbers(p);

    bool res = true;
    res = res && statementNums.size() == 0;
    REQUIRE(res);
}

TEST_CASE("Given a query for a procedure and a statementNumber that is not nested in the procedure, checkProcedure() returns false") {
    ProcedureStorage store;
    Proc p = "computeCentroid";
    std::vector<StmtNum> lines;
    lines.push_back(2);
    store.writeProcedure(p, lines);

    bool res = true;
    res = res && (store.checkProcedure(p, 4) == false);
    REQUIRE(res);
}

TEST_CASE("If a procedure does not exist, checkProcedure() returns false") {
    ProcedureStorage store;
    bool res = (store.checkProcedure("procedureThatDoesNotExist", 4) == false);
    REQUIRE(res);
}
