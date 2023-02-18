#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("findDesignEntities() Tests") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::unordered_set<StmtNum> lines = {3, 6, 9};

    StmtNum stmtNum = 123;
    writePkb.setConstant(stmtNum, {123});

    ProcName p = "Main";
    writePkb.setProcedure(p, lines);

    Stmt s = "if";
    writePkb.setStatement(s, lines);

    Ent e = "varName";
    writePkb.setEntity(1, {e});

    Parameter constParam = Parameter("irrelevant", "constant");
    std::vector<std::string> res = readPkb.findDesignEntities(constParam);
    std::vector<std::string> check = { "123" };
    REQUIRE(res == check);

    Parameter procParam = Parameter("irrelevant", "procedure");
    res = readPkb.findDesignEntities(procParam);
    check = { "Main" };
    REQUIRE(res == check);

    Parameter ifParam = Parameter("irrelevant", "if");
    res = readPkb.findDesignEntities(ifParam);
    check = { "3", "6", "9" };
    std::sort(res.begin(), res.end());
    std::sort(check.begin(), check.end());
    REQUIRE(res == check);

    Parameter entParam = Parameter("irrelevant", "variable");
    res = readPkb.findDesignEntities(entParam);
    check = {"varName"};
    REQUIRE(res == check);
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


