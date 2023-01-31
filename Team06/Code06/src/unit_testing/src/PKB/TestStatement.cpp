#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("Check that all statements are recorded") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    std::vector<StmtNum> lines;
    lines.push_back(3);
    lines.push_back(6);
    lines.push_back(9);
    writePkb.setStatement(s, lines);
    std::unordered_set<StmtNum> statementNums = readPkb.getStatementNumbers(s);

    bool res = true;
    res = res && statementNums.size() == 3;
    res = res && statementNums.find(3) != statementNums.end(); //3 is present
    res = res && statementNums.find(6) != statementNums.end(); //6 is present
    res = res && statementNums.find(9) != statementNums.end(); //9 is present
    res = res && statementNums.find(12) == statementNums.end(); //12 is not present
    REQUIRE(res);
}

TEST_CASE("Check that a statement can have zero statement nums") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    std::vector<StmtNum> lines;
    writePkb.setStatement(s, lines);
    std::unordered_set<StmtNum> statementNums = readPkb.getStatementNumbers(s);

    bool res = true;
    res = res && statementNums.size() == 0;
    res = res && statementNums.find(3) == statementNums.end(); //3 is not present
    res = res && statementNums.find(12) == statementNums.end(); //12 is not present
    REQUIRE(res);
}

TEST_CASE("Check that if a statement is not in the source code, it returns an empty set") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    std::vector<StmtNum> lines;
    writePkb.setStatement(s, lines);
    std::unordered_set<StmtNum> statementNums = readPkb.getStatementNumbers("while");

    REQUIRE(statementNums.size() == 0);
}

TEST_CASE("Check that given a statement and their statement numbers, a query for the statement and its statement number returns the right boolean") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    std::vector<StmtNum> lines;
    lines.push_back(3);
    writePkb.setStatement(s, lines);

    bool res = true;
    res = res && (readPkb.checkStatement("if", 3) == true);
    res = res && (readPkb.checkStatement("if", 4) == false);
    REQUIRE(res);
}

TEST_CASE("Check that given a statement and their statement numbers, a query for an non-existent statement returns false") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    std::vector<StmtNum> lines;
    lines.push_back(3);
    writePkb.setStatement(s, lines);

    REQUIRE(readPkb.checkStatement("while", 4) == false);
}

