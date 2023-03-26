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
    std::vector<Parameter> params1 = {Parameter("1", ParameterType::FIXED_INT),
                                      Parameter("a", ParameterType::FIXED_STRING)};
    shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::MODIFIES, params1);

    SECTION("findRelationship(shared_ptr<Relationship> rs): empty storage") {
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(res.size() == 0);
    }

    std::unordered_set<Ent> val1 = {"a", "b", "c"};
    std::unordered_set<Ent> val2 = {"a"};
    std::unordered_set<Ent> val3 = {"a", "b"};
    writePkb.setModifiesS(1, val1);
    writePkb.setModifiesS(2, val2);
    writePkb.setModifiesS(3, val3);
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
        std::vector<Parameter> params2 = {Parameter("1", ParameterType::FIXED_INT),
                                          Parameter("nonExist", ParameterType::FIXED_STRING)};
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
        std::vector<Parameter> params2 = {Parameter("100000", ParameterType::FIXED_INT),
                                          Parameter("a", ParameterType::FIXED_STRING)};
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
        std::vector<Parameter> params2 = {Parameter("100000", ParameterType::FIXED_INT),
                                          Parameter("nonExist", ParameterType::FIXED_STRING)};
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
        std::vector<Parameter> params2 = {Parameter("1", ParameterType::FIXED_INT),
                                          Parameter("_", ParameterType::WILDCARD)};
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
        std::vector<Parameter> params2 = {Parameter("100000", ParameterType::FIXED_INT),
                                          Parameter("_", ParameterType::WILDCARD)};
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
        std::vector<Parameter> params2 = {Parameter("s", ParameterType::STMT), Parameter("_", ParameterType::WILDCARD)};
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
        std::vector<Parameter> params2 = {Parameter("s", ParameterType::STMT),
                                          Parameter("a", ParameterType::FIXED_STRING)};
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
        std::vector<Parameter> params2 = {Parameter("s", ParameterType::STMT),
                                          Parameter("nonExist", ParameterType::FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }
}

//  : by default, all are failing since it is not part of Milestone 1's requirements. it is part of our TDD.
TEST_CASE("findRelationship(shared_ptr<Relationship> rs), Modifies / Uses P") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    // Modifies ("proc1", "a")
    std::vector<Parameter> params1 = {Parameter("proc1", ParameterType::FIXED_STRING),
                                      Parameter("a", ParameterType::FIXED_STRING)};
    shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::MODIFIES, params1);

    SECTION("findRelationship(shared_ptr<Relationship> rs): empty storage") {
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(res.size() == 0);
    }

    std::unordered_set<Ent> val1 = {"a", "b", "c"};
    std::unordered_set<Ent> val2 = {"a"};
    std::unordered_set<Ent> val3 = {"a", "b"};
    writePkb.setModifiesP("proc1", val1);
    writePkb.setModifiesP("proc2", val2);
    writePkb.setModifiesP("proc3", val3);

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
        std::vector<Parameter> params2 = {Parameter("proc1", ParameterType::FIXED_STRING),
                                          Parameter("nonExist", ParameterType::FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);
        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        REQUIRE(equals(actual, expected));
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: non-existent "
            "procedure, RHS: non-wildcard") {
        // Modifies ("nonExist", "c")
        std::vector<Parameter> params2 = {Parameter("nonExist", ParameterType::FIXED_STRING),
                                          Parameter("c", ParameterType::FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);
        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        REQUIRE(equals(actual, expected));
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: ProcName, RHS: "
            "variable") {
        // Modifies ("proc1", v)
        std::vector<Parameter> params2 = {Parameter("proc1", ParameterType::FIXED_STRING),
                                          Parameter("v", ParameterType::VARIABLE)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {{"proc1", "a"}, {"proc1", "b"}, {"proc1", "c"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        REQUIRE(equals(actual, expected));
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: ProcName, RHS: "
            "wildcard") {
        // Modifies ("proc1", _)
        std::vector<Parameter> params2 = {Parameter("proc1", ParameterType::FIXED_STRING),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {{"proc1", "a"}, {"proc1", "b"}, {"proc1", "c"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        REQUIRE(equals(actual, expected));
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: non-existent, RHS: "
            "wildcard") {
        // Modifies ("nonExist", _)
        std::vector<Parameter> params2 = {Parameter("nonExist", ParameterType::FIXED_STRING),
                                          Parameter("_", ParameterType::WILDCARD)};
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
    std::unordered_set<Const> val1 = {"123"};
    writePkb.setConstant(stmtNum, val1);

    ProcName p = "Main";
    writePkb.setProcedure(p, lines);

    Stmt s = "if";
    writePkb.setStatement(s, 3);
    writePkb.setStatement(s, 6);
    writePkb.setStatement(s, 9);

    Ent e = "varName";
    std::unordered_set<Ent> val2 = {e};
    writePkb.setEntity(1, val2);

    Parameter constParam = Parameter("irrelevant", ParameterType::CONSTANT);
    std::vector<std::string> res = readPkb.findDesignEntities(constParam);
    std::vector<std::string> check = {"123"};
    REQUIRE(res == check);

    Parameter procParam = Parameter("irrelevant", ParameterType::PROCEDURE);
    res = readPkb.findDesignEntities(procParam);
    check = {"Main"};
    REQUIRE(res == check);

    Parameter ifParam = Parameter("irrelevant", ParameterType::IF);
    res = readPkb.findDesignEntities(ifParam);
    check = {"3", "6", "9"};
    std::sort(res.begin(), res.end());
    std::sort(check.begin(), check.end());
    REQUIRE(res == check);

    Parameter entParam = Parameter("irrelevant", ParameterType::VARIABLE);
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

    Parameter p = Parameter("irrelevant", ParameterType::IF);
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
    Parameter p = Parameter(s, ParameterType::STMT);
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

    std::unordered_set<StmtNum> val1 = {1, 2, 3, 4, 5};
    std::unordered_set<StmtNum> val2 = {6, 7, 8};
    writePkb.setProcedure("proc1", val1);
    writePkb.setProcedure("proc2", val2);

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

    std::unordered_set<Ent> val1 = {"a", "b", "c"};
    std::unordered_set<Ent> val2 = {"a"};
    std::unordered_set<Ent> val3 = {"b", "x", "z"};
    std::unordered_set<Ent> val4 = {"a", "b", "x", "z"};
    writePkb.setUsesS(1, val1);
    writePkb.setUsesS(2, val2);
    writePkb.setUsesS(3, val2);
    writePkb.setUsesS(4, val3);
    writePkb.setUsesP("proc1", val1);
    writePkb.setUsesP("proc2", val4);

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


    std::unordered_set<Ent> val1 = {"a", "b", "c"};
    std::unordered_set<Ent> val2 = {"a"};
    std::unordered_set<Ent> val3 = {"b", "x", "z"};
    std::unordered_set<Ent> val4 = {"a", "b", "x", "z"};
    writePkb.setModifiesS(1, val1);
    writePkb.setModifiesS(2, val2);
    writePkb.setModifiesS(3, val2);
    writePkb.setModifiesS(4, val3);
    writePkb.setModifiesP("proc1", val1);
    writePkb.setModifiesP("proc2", val4);

    SECTION("ModifiesStorage WritePKB ReadPKB Facade: getModifiesS(StmtNum num) positive case") {
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

    SECTION("ModifiesStorage WritePKB ReadPKB Facade: getModifiesS(StmtNum num) negative case") {
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
        REQUIRE(expected == actual);
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

    std::unordered_set<StmtNum> val1 = {12, 13, 14, 15};
    std::unordered_set<StmtNum> val2 = {13};
    std::unordered_set<StmtNum> val3 = {15};
    writePkb.setParentT(11, val1);
    writePkb.setParentT(12, val2);
    writePkb.setParentT(14, val3);

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
    std::unordered_set<ProcName> val1 = {proc1, proc2};
    writePkb.setCalls(caller, val1);
    writePkb.setCallsT(caller, callees);
}
