#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;

// findRelationship() tests
// Modifies and Uses has the same storage structure and logic thus these test cases that covers the relationship for
// Modifies is applicable to Uses also since they have the same function call signature
TEST_CASE("findRelationship(shared_ptr<Relationship> rs), Modifies / Uses S") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    // Modifies (1, "a")
    std::vector<Parameter> params1 = {Parameter("1", AppConstants::FIXED_INT),
                                      Parameter("a", AppConstants::FIXED_STRING)};
    shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::MODIFIES, params1);

    SECTION("findRelationship(shared_ptr<Relationship> rs): empty storage") {
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(res.size() == 0);
    }

    writePkb.setModifiesS(1, {"a", "b", "c"});
    writePkb.setModifiesS(2, {"a"});
    writePkb.setModifiesS(3, {"a", "b"});
    writePkb.setStatement(AppConstants::IF, 1);
    writePkb.setStatement(AppConstants::WHILE, 2);
    writePkb.setStatement(AppConstants::ASSIGN, 3);

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: StmtNum, RHS: "
            "non-wildcard") {
        // Modifies (1, "a")
        std::vector<std::vector<std::string>> expected = {{"1", "a"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs1);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: StmtNum, RHS: "
            "non-exist") {
        // Modifies (1, "nonExist")
        std::vector<Parameter> params2 = {Parameter("1", AppConstants::FIXED_INT),
                                          Parameter("nonExist", AppConstants::FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);
        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: non-exist, RHS: "
            "non-wildcard") {
        // Modifies (nonExist, "a")
        std::vector<Parameter> params2 = {Parameter("100000", AppConstants::FIXED_INT),
                                          Parameter("a", AppConstants::FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);
        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: non-exist, RHS: "
            "non-exist") {
        // Modifies (nonExist, nonExist)
        std::vector<Parameter> params2 = {Parameter("100000", AppConstants::FIXED_INT),
                                          Parameter("nonExist", AppConstants::FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);
        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: StmtNum, RHS: "
            "wildcard") {
        // Modifies (1, _)
        std::vector<Parameter> params2 = {Parameter("1", AppConstants::FIXED_INT),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {{"1", "a"}, {"1", "b"}, {"1", "c"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: non-exist, RHS: "
            "wildcard") {
        // Modifies (nonExist, _)
        std::vector<Parameter> params2 = {Parameter("100000", AppConstants::FIXED_INT),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION(
        "findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: Stmt, RHS: wildcard") {
        // Modifies (s, _)
        std::vector<Parameter> params2 = {Parameter("s", AppConstants::STMT), Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {{"1", "a"}, {"1", "b"}, {"1", "c"},
                                                          {"2", "a"}, {"3", "a"}, {"3", "b"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: Stmt, RHS: "
            "non-wildcard") {
        // Modifies (s, "a")
        std::vector<Parameter> params2 = {Parameter("s", AppConstants::STMT),
                                          Parameter("a", AppConstants::FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {{"1", "a"}, {"2", "a"}, {"3", "a"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION(
        "findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: Stmt, RHS: nonExist") {
        // Modifies (s, "nonExist")
        std::vector<Parameter> params2 = {Parameter("s", AppConstants::STMT),
                                          Parameter("nonExist", AppConstants::FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }
}

// TODO: by default, all are failing since it is not part of Milestone 1's requirements. it is part of our TDD.
TEST_CASE("findRelationship(shared_ptr<Relationship> rs), Modifies / Uses P") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    // Modifies ("proc1", "a")
    std::vector<Parameter> params1 = {Parameter("proc1", AppConstants::FIXED_STRING),
                                      Parameter("a", AppConstants::FIXED_STRING)};
    shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::MODIFIES, params1);

    SECTION("findRelationship(shared_ptr<Relationship> rs): empty storage") {
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(res.size() == 0);
    }

    writePkb.setModifiesP("proc1", {"a", "b", "c"});
    writePkb.setModifiesP("proc2", {"a"});
    writePkb.setModifiesP("proc3", {"a", "b"});

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: ProcName, RHS: "
            "non-wildcard") {
        // Modifies ("proc1", "a")
        std::vector<std::vector<std::string>> expected = {{"proc1", "a"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs1);
        REQUIRE(equals(actual, expected));
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: ProcName, "
            "RHS:non-existent entity") {
        // Modifies ("proc1", "nonExist")
        std::vector<Parameter> params2 = {Parameter("proc1", AppConstants::FIXED_STRING),
                                          Parameter("nonExist", AppConstants::FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);
        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        REQUIRE(equals(actual, expected));
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: non-existent "
            "procedure, RHS: non-wildcard") {
        // Modifies ("nonExist", "c")
        std::vector<Parameter> params2 = {Parameter("nonExist", AppConstants::FIXED_STRING),
                                          Parameter("c", AppConstants::FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);
        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        REQUIRE(equals(actual, expected));
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: ProcName, RHS: "
            "variable") {
        // Modifies ("proc1", v)
        std::vector<Parameter> params2 = {Parameter("proc1", AppConstants::FIXED_STRING),
                                          Parameter("v", AppConstants::VARIABLE)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {{"proc1", "a"}, {"proc1", "b"}, {"proc1", "c"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        REQUIRE(equals(actual, expected));
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: ProcName, RHS: "
            "wildcard") {
        // Modifies ("proc1", _)
        std::vector<Parameter> params2 = {Parameter("proc1", AppConstants::FIXED_STRING),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {{"proc1", "a"}, {"proc1", "b"}, {"proc1", "c"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        REQUIRE(equals(actual, expected));
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: non-existent, RHS: "
            "wildcard") {
        // Modifies ("nonExist", _)
        std::vector<Parameter> params2 = {Parameter("nonExist", AppConstants::FIXED_STRING),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        REQUIRE(equals(actual, expected));
    }
}

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
    writePkb.setStatement(s, 3);
    writePkb.setStatement(s, 6);
    writePkb.setStatement(s, 9);

    Ent e = "varName";
    writePkb.setEntity(1, {e});

    Parameter constParam = Parameter("irrelevant", AppConstants::CONSTANT);
    std::vector<std::string> res = readPkb.findDesignEntities(constParam);
    std::vector<std::string> check = {"123"};
    REQUIRE(res == check);

    Parameter procParam = Parameter("irrelevant", AppConstants::PROCEDURE);
    res = readPkb.findDesignEntities(procParam);
    check = {"Main"};
    REQUIRE(res == check);

    Parameter ifParam = Parameter("irrelevant", AppConstants::IF);
    res = readPkb.findDesignEntities(ifParam);
    check = {"3", "6", "9"};
    std::sort(res.begin(), res.end());
    std::sort(check.begin(), check.end());
    REQUIRE(res == check);

    Parameter entParam = Parameter("irrelevant", AppConstants::VARIABLE);
    res = readPkb.findDesignEntities(entParam);
    check = {"varName"};
    REQUIRE(res == check);
}

// findDesignEntities() tests
TEST_CASE("Check that ReadPKB returns all statement numbers of a given statement type") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    Stmt s = "if";
    writePkb.setStatement(s, 3);
    writePkb.setStatement(s, 6);
    writePkb.setStatement(s, 9);

    Parameter p = Parameter("irrelevant", s);
    std::vector<std::string> res = readPkb.findDesignEntities(p);
    std::vector<std::string> check = {"3", "6", "9"};

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
    Parameter p = Parameter(s, "irrelevant");
    std::vector<std::string> res = readPkb.findDesignEntities(p);
    std::vector<std::string> check;

    REQUIRE(res == check);
}

TEST_CASE("CallStorage WritePKB ReadPKB Facade") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setCall(11, "proc2");
    writePkb.setCall(22, "proc3");

    SECTION("CallStorage WritePKB ReadPKB Facade: getCallStmt(StmtNum s)") {
        auto res = readPkb.getCallStmt(11);
        REQUIRE(res.first == 11);
        REQUIRE(res.second == "proc2");
    }

    SECTION("CallStorage WritePKB ReadPKB Facade: getCallStatements()") {
        auto actual = readPkb.getCallStatements();
        std::vector<std::pair<StmtNum, ProcName>> expected = {{11, "proc2"}, {22, "proc3"}};
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }
}

TEST_CASE("StmtStorage WritePKB ReadPKB Facade") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setStatement(AppConstants::IF, 11);
    writePkb.setStatement(AppConstants::IF, 111);
    writePkb.setStatement(AppConstants::IF, 112);
    writePkb.setStatement(AppConstants::WHILE, 12);
    writePkb.setStatement(AppConstants::WHILE, 222);
    writePkb.setStatement(AppConstants::WHILE, 333);
    writePkb.setStatement(AppConstants::ASSIGN, 13);
    writePkb.setStatement(AppConstants::PRINT, 20);
    writePkb.setStatement(AppConstants::READ, 2);
    writePkb.setStatement(AppConstants::CALL, 7);
    writePkb.setStatement(AppConstants::CALL, 10);

    SECTION("StmtStorage WritePKB ReadPKB Facade: checkStatement(Stmt stmt, StmtNum num)") {
        REQUIRE(readPkb.checkStatement(AppConstants::IF, 11));
        REQUIRE(readPkb.checkStatement(AppConstants::CALL, 10));
        REQUIRE(readPkb.checkStatement(AppConstants::CALL, 7));
        REQUIRE(!readPkb.checkStatement(AppConstants::WHILE, 11));
        REQUIRE(!readPkb.checkStatement(AppConstants::WHILE, 2));
    }

    SECTION("StmtStorage WritePKB ReadPKB Facade: getIfStatementNumbers()") {
        std::vector<StmtNum> expected = {11, 111, 112};
        auto res = readPkb.getIfStatementNumbers();
        std::vector<StmtNum> actual;
        for (StmtNum num : res) {
            actual.push_back(num);
        }
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("StmtStorage WritePKB ReadPKB Facade: getWhileStatementNumbers()") {
        std::vector<StmtNum> expected = {12, 222, 333};
        auto res = readPkb.getWhileStatementNumbers();
        std::vector<StmtNum> actual;
        for (StmtNum num : res) {
            actual.push_back(num);
        }
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }
}

TEST_CASE("ProcedureStorage WritePKB ReadPKB Facade") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setProcedure("proc1", {1, 2, 3, 4, 5});
    writePkb.setProcedure("proc2", {6, 7, 8});

    SECTION("ProcedureStorage WritePKB ReadPKB Facade: getProcedureStatementNumbers(ProcName p) positive case") {
        std::vector<StmtNum> expected = {1, 2, 3, 4, 5};
        auto res = readPkb.getProcedureStatementNumbers("proc1");
        std::vector<StmtNum> actual;
        for (StmtNum num : res) {
            actual.push_back(num);
        }
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("ProcedureStorage WritePKB ReadPKB Facade: getProcedureStatementNumbers(ProcName p) negative case") {
        std::vector<StmtNum> expected = {};
        auto res = readPkb.getProcedureStatementNumbers("doesNotExist");
        REQUIRE(res.empty());
    }

    SECTION("ProcedureStorage WritePKB ReadPKB Facade: getAllProcedureNames() positive case") {
        std::vector<ProcName> expected = {"proc1", "proc2"};
        auto res = readPkb.getAllProcedureNames();
        std::vector<ProcName> actual;
        for (ProcName name : res) {
            actual.push_back(name);
        }
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }
}

TEST_CASE("UsesStorage WritePKB ReadPKB Facade") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setUsesS(1, {"a", "b", "c"});
    writePkb.setUsesS(2, {"a"});
    writePkb.setUsesS(3, {"a"});
    writePkb.setUsesS(4, {"b", "x", "z"});
    writePkb.setUsesP("proc1", {"a", "b", "c"});
    writePkb.setUsesP("proc2", {"a", "b", "x", "z"});

    SECTION("UsesStorage WritePKB ReadPKB Facade: getUsesS(StmtNum num) positive case") {
        std::vector<Ent> expected = {"a", "b", "c"};
        auto res = readPkb.getUsesS(1);
        std::vector<Ent> actual;
        for (ProcName name : res) {
            actual.push_back(name);
        }
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("UsesStorage WritePKB ReadPKB Facade: getUsesS(StmtNum num) negative case") {
        auto res = readPkb.getUsesS(11);
        REQUIRE(res.empty());
    }

    SECTION("UsesStorage WritePKB ReadPKB Facade: getUsesP(ProcName name) positive case") {
        std::vector<Ent> expected = {"a", "b", "x", "z"};
        auto res = readPkb.getUsesP("proc2");
        std::vector<Ent> actual;
        for (ProcName name : res) {
            actual.push_back(name);
        }
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("UsesStorage WritePKB ReadPKB Facade: getUsesP(ProcName name) negative case") {
        auto res = readPkb.getUsesP("proc3");
        REQUIRE(res.empty());
    }
}

TEST_CASE("ModifiesStorage WritePKB ReadPKB Facade") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setModifiesS(1, {"a", "b", "c"});
    writePkb.setModifiesS(2, {"a"});
    writePkb.setModifiesS(3, {"a"});
    writePkb.setModifiesS(4, {"b", "x", "z"});
    writePkb.setModifiesP("proc1", {"a", "b", "c"});
    writePkb.setModifiesP("proc2", {"a", "b", "x", "z"});

    SECTION("ModifiesStorage WritePKB ReadPKB Facade: getUsesS(StmtNum num) positive case") {
        std::vector<Ent> expected = {"a", "b", "c"};
        auto res = readPkb.getModifiesS(1);
        std::vector<Ent> actual;
        for (ProcName name : res) {
            actual.push_back(name);
        }
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("ModifiesStorage WritePKB ReadPKB Facade: getUsesS(StmtNum num) negative case") {
        auto res = readPkb.getModifiesS(11);
        REQUIRE(res.empty());
    }

    SECTION("ModifiesStorage WritePKB ReadPKB Facade: getUsesP(ProcName name) positive case") {
        std::vector<Ent> expected = {"a", "b", "x", "z"};
        auto res = readPkb.getModifiesP("proc2");
        std::vector<Ent> actual;
        for (ProcName name : res) {
            actual.push_back(name);
        }
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("ModifiesStorage WritePKB ReadPKB Facade: getUsesP(ProcName name) negative case") {
        auto res = readPkb.getModifiesP("proc3");
        REQUIRE(res.empty());
    }
}

TEST_CASE("ParentTStorage WritePKB ReadPKB Facade") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParentT(11, {12, 13, 14, 15});
    writePkb.setParentT(12, {13});
    writePkb.setParentT(14, {15});

    SECTION("ParentTStorage WritePKB ReadPKB Facade: getContainedStatements(StmtNum containerNum) positive case") {
        std::vector<StmtNum> expected = {12, 13, 14, 15};
        auto res = readPkb.getContainedStatements(11);
        std::vector<StmtNum> actual;
        for (StmtNum num : res) {
            actual.push_back(num);
        }
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("ParentTStorage WritePKB ReadPKB Facade: getContainedStatements(StmtNum containerNum) negative case") {
        auto res = readPkb.getContainedStatements(15);
        REQUIRE(res.empty());
    }
}

TEST_CASE("CallStorage WritePKB ReadPKB Facade procedure names") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    ProcName caller = "proc0";
    ProcName proc1 = "proc1";
    ProcName proc2 = "proc2";
    std::unordered_set<ProcName> callees = {proc1, proc2};

    writePkb.setCall(11, proc1);
    writePkb.setCall(21, proc2);
    writePkb.setCalls(caller, {proc1, proc2});
    writePkb.setCallsT(caller, callees);
}
