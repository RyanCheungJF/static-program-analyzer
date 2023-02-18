#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/utils/AppConstants.h"

AppConstants CONSTANTS;

//findRelationship() tests
//Modifies and Uses has the same storage structure and logic thus these test cases that covers the relationship for
//Modifies is applicable to Uses also since they have the same function call signature
TEST_CASE("findRelationship(shared_ptr<Relationship> rs), Modifies / Uses S") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    // Modifies (1, "a")
    std::vector<Parameter> params1 = {Parameter("1", CONSTANTS.FIXED_INT), Parameter("a", CONSTANTS.FIXED_STRING)};
    shared_ptr<Relationship> rs1 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params1);

    SECTION("findRelationship(shared_ptr<Relationship> rs): empty storage") {
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(res.size() == 0);
    }

    writePkb.setModifiesS(1, {"a", "b", "c"});
    writePkb.setModifiesS(2, {"a"});
    writePkb.setModifiesS(3, {"a", "b"});
    writePkb.setStatement(CONSTANTS.IF, 1);
    writePkb.setStatement(CONSTANTS.WHILE, 2);
    writePkb.setStatement(CONSTANTS.ASSIGN, 3);


    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: StmtNum, RHS: non-wildcard") {
        // Modifies (1, "a")
        std::vector<std::vector<std::string>> expected = {{"1", "a"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs1);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: StmtNum, RHS: non-exist") {
        // Modifies (1, "nonExist")
        std::vector<Parameter> params2 = {Parameter("1", CONSTANTS.FIXED_INT), Parameter("nonExist", CONSTANTS.FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params2);
        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: non-exist, RHS: non-wildcard") {
        // Modifies (nonExist, "a")
        std::vector<Parameter> params2 = {Parameter("100000", CONSTANTS.FIXED_INT), Parameter("a", CONSTANTS.FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params2);
        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: non-exist, RHS: non-exist") {
        // Modifies (nonExist, nonExist)
        std::vector<Parameter> params2 = {Parameter("100000", CONSTANTS.FIXED_INT), Parameter("nonExist", CONSTANTS.FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params2);
        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: StmtNum, RHS: wildcard") {
        // Modifies (1, _)
        std::vector<Parameter> params2 = {Parameter("1", CONSTANTS.FIXED_INT), Parameter("_", CONSTANTS.WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {{"1", "a"}, {"1", "b"}, {"1", "c"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: non-exist, RHS: wildcard") {
        // Modifies (nonExist, _)
        std::vector<Parameter> params2 = {Parameter("100000", CONSTANTS.FIXED_INT), Parameter("_", CONSTANTS.WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: Stmt, RHS: wildcard") {
        // Modifies (s, _)
        std::vector<Parameter> params2 = {Parameter("s", CONSTANTS.STMT), Parameter("_", CONSTANTS.WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {{"1", "a"}, {"1", "b"}, {"1", "c"},
                                                          {"2", "a"}, {"3", "a"}, {"3", "b"}
        };
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: Stmt, RHS: non-wildcard") {
        // Modifies (s, "a")
        std::vector<Parameter> params2 = {Parameter("s", CONSTANTS.STMT), Parameter("a", CONSTANTS.FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {{"1", "a"}, {"2", "a"}, {"3", "a"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: Stmt, RHS: nonExist") {
        // Modifies (s, "nonExist")
        std::vector<Parameter> params2 = {Parameter("s", CONSTANTS.STMT), Parameter("nonExist", CONSTANTS.FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }
}

/*
// TODO: by default, all are failing since it is not part of Milestone 1's requirements. it is part of our TDD.
TEST_CASE("findRelationship(shared_ptr<Relationship> rs), Modifies / Uses P") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    // Modifies ("proc1", "a")
    std::vector<Parameter> params1 = {Parameter("proc1", CONSTANTS.PROCEDURE), Parameter("a", CONSTANTS.FIXED_STRING)};
    shared_ptr<Relationship> rs1 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params1);

    SECTION("findRelationship(shared_ptr<Relationship> rs): empty storage") {
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(res.size() == 0);
    }

    writePkb.setModifiesP("proc1", {"a", "b", "c"});
    writePkb.setModifiesP("proc2", {"a"});
    writePkb.setModifiesP("proc3", {"a", "b"});

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: ProcName, RHS: non-wildcard") {
        // TODO: or is it everything since the query evaluates to true?
        std::vector<std::vector<std::string>> expected = {{"proc1", "a"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs1);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }


    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: ProcName, RHS: non-existent entity") {
        // Modifies ("proc1", "nonExist")
        std::vector<Parameter> params2 = {Parameter("proc1", CONSTANTS.PROCEDURE), Parameter("nonExist", CONSTANTS.FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params2);
        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: non-existent procedure, RHS: non-wildcard") {
        // Modifies ("nonExist", "c")
        std::vector<Parameter> params2 = {Parameter("nonExist", CONSTANTS.PROCEDURE), Parameter("c", CONSTANTS.FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params2);
        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: non-existent procedure, RHS: non-existent entity") {
        // Modifies ("nonExist", "nonExist")
        std::vector<Parameter> params2 = {Parameter("nonExist", CONSTANTS.PROCEDURE), Parameter("nonExist", CONSTANTS.FIXED_STRING)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params2);
        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, positive case, LHS: ProcName, RHS: wildcard") {
        // Modifies ("proc1", _)
        std::vector<Parameter> params2 = {Parameter("proc1", CONSTANTS.PROCEDURE), Parameter("_", CONSTANTS.WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = { { "proc1", "a" }, { "proc1", "b" }, { "proc1", "c" } };
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(shared_ptr<Relationship> rs): non-empty storage, negative case, LHS: non-existent, RHS: wildcard") {
        // Modifies ("nonExist", _)
        std::vector<Parameter> params2 = {Parameter("nonExist", CONSTANTS.PROCEDURE), Parameter("_", CONSTANTS.WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(CONSTANTS.MODIFIES, params2);

        std::vector<std::vector<std::string>> expected = { { "proc1", "a" }, { "proc1", "b" }, { "proc1", "c" } };
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }
}
*/


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
    writePkb.setStatement(s, 3);
    writePkb.setStatement(s, 6);
    writePkb.setStatement(s, 9);

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
    Parameter p = Parameter(s, "irrelevant");
    std::vector<std::string> res = readPkb.findDesignEntities(p);
    std::vector<std::string> check;

    REQUIRE(res == check);
}


