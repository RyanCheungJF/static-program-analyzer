#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("Check that all statements are recorded in StmtStorage") {
    StmtStorage sts;

    Stmt s = "if";
    std::unordered_set<StmtNum> lines = {3, 6, 9};
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
    std::unordered_set<StmtNum> lines;
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
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    std::unordered_set<StmtNum> lines = {3};
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
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    bool res = (readPkb.checkStatement("if", 4) == false);
    REQUIRE(res);
}

TEST_CASE("Check that given a StatementAPI and their StatementAPI numbers, a query for an non-existent StatementAPI returns false") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    std::unordered_set<StmtNum> lines = {3};
    writePkb.setStatement(s, lines);

    REQUIRE(readPkb.checkStatement("while", 4) == false);
}

//findDesignEntities() tests
TEST_CASE("Check that ReadPKB returns all statement numbers of a given statement type") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    std::unordered_set<StmtNum> lines = {3, 6, 9};
    writePkb.setStatement(s, lines);

    Parameter p = Parameter("irrelevant", s);
    std::vector<std::string> res = readPkb.findDesignEntities(p);
    std::vector<std::string> check = { "3", "6", "9" };

    std::sort(res.begin(), res.end());
    std::sort(check.begin(), check.end());
    REQUIRE(res == check);
}

TEST_CASE("Check that a statement does not appear in the source code, ReadPKB should return an empty vector") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    std::unordered_set<StmtNum> lines;
    writePkb.setStatement(s, lines);
    Parameter p = Parameter(s, "irrelevant");
    std::vector<std::string> res = readPkb.findDesignEntities(p);
    std::vector<std::string> check;

    REQUIRE(res == check);
}

TEST_CASE("StmtStorage: getStatementNumbers(Stmt s): stmt") {
    StmtStorage sts;
    Stmt stmt = "stmt";

    SECTION("getStatementNumbers(Stmt s): empty storage") {
        std::unordered_set<StmtNum> statementNums = sts.getStatementNumbers(stmt);
        REQUIRE(statementNums.size() == 0);

    }
    Stmt s1 = "if";
    std::unordered_set<StmtNum> lines1 = {3, 6, 9};
    sts.writeStatement(s1, lines1);

    Stmt s2 = "while";
    std::unordered_set<StmtNum> lines2 = {17, 22, 24};
    sts.writeStatement(s2, lines2);

    Stmt s3 = "assign";
    std::unordered_set<StmtNum> lines3 = {5, 7};
    sts.writeStatement(s3, lines3);

    Stmt s4 = "print";
    std::unordered_set<StmtNum> lines4 = {4};
    sts.writeStatement(s4, lines4);

    Stmt s5 = "read";
    std::unordered_set<StmtNum> lines5 = {8, 18, 20};
    sts.writeStatement(s5, lines5);

    Stmt s6 = "call";
    std::unordered_set<StmtNum> lines6 = {23};
    sts.writeStatement(s6, lines6);

    SECTION("getStatementNumbers(Stmt s): non-empty storage") {
        std::unordered_set<StmtNum> statementNums = sts.getStatementNumbers(stmt);
        REQUIRE(statementNums.size() == 13);
    }
}
