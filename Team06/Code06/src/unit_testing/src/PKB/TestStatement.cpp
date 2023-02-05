#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("Check that all statements are recorded in StmtStorage") {
    StmtStorage sts;

    Stmt s = "if";
    std::vector<StmtNum> lines;
    lines.push_back(3);
    lines.push_back(6);
    lines.push_back(9);
    sts.writeStatement(s, lines);
    std::unordered_set<StmtNum> statementNums = sts.getStatementNumbers(s);

    bool res = true;
    res = res && statementNums.size() == 3;
    res = res && statementNums.find(3) != statementNums.end(); //3 is present
    res = res && statementNums.find(6) != statementNums.end(); //6 is present
    res = res && statementNums.find(9) != statementNums.end(); //9 is present
    res = res && statementNums.find(12) == statementNums.end(); //12 is not present
    REQUIRE(res);
}

TEST_CASE("Check that a statement does not appear in the source code, StmtStorage should return an empty set") {
    StmtStorage sts;

    Stmt s = "if";
    std::vector<StmtNum> lines;
    sts.writeStatement(s, lines);
    std::unordered_set<StmtNum> statementNums = sts.getStatementNumbers(s);

    bool res = true;
    res = res && statementNums.size() == 0;
    res = res && statementNums.find(3) == statementNums.end(); //3 is not present
    res = res && statementNums.find(12) == statementNums.end(); //12 is not present
    REQUIRE(res);
}

//Might not need to support checkStatement()
TEST_CASE("Check that given query for a statement and a statementNumber that it appears in, it returns true") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    StmtStorage st;
    pkb.statementStorage = &st;
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

// Might not need to support checkStatement()
TEST_CASE("Check that if a statement does not exist, t returns false") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    StmtStorage st;
    pkb.statementStorage = &st;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    bool res = (readPkb.checkStatement("if", 4) == false);
    REQUIRE(res);
}

TEST_CASE("Check that given a StatementAPI and their StatementAPI numbers, a query for an non-existent StatementAPI returns false") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    StmtStorage st;
    pkb.statementStorage = &st;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    std::vector<StmtNum> lines;
    lines.push_back(3);
    writePkb.setStatement(s, lines);

    REQUIRE(readPkb.checkStatement("while", 4) == false);
}

//findDesignEntities() tests
TEST_CASE("Check that ReadPKB returns all statement numbers of a given statement type") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    StmtStorage st;
    pkb.statementStorage = &st;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    std::vector<StmtNum> lines;
    lines.push_back(3);
    lines.push_back(6);
    lines.push_back(9);
    writePkb.setStatement(s, lines);

    Parameter p = Parameter("irrelevant", s);
    std::vector<std::string> res = readPkb.findDesignEntities(p);
    std::vector<std::string> check = { "3", "6", "9" };

    REQUIRE(res == check);
}

TEST_CASE("Check that a statement does not appear in the source code, ReadPKB should return an empty vector") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    StmtStorage st;
    pkb.statementStorage = &st;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    std::vector<StmtNum> lines;
    writePkb.setStatement(s, lines);
    Parameter p = Parameter(s, "irrelevant");
    std::vector<std::string> res = readPkb.findDesignEntities(p);
    std::vector<std::string> check;

    REQUIRE(res == check);
}
