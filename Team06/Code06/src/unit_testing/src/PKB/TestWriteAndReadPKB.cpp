#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"


//findRelationship() tests
//Modifies and Uses has the same storage structure and logic thus these test cases that covers the relationship for
//Modifies is applicable to Uses also
TEST_CASE("findRelationship(Relationship rs)") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    // Modifies (1, "a")
    std::vector<Parameter> params1 = {Parameter("1", "fixed_int"), Parameter("a", "fixed_string")};
    Relationship rs1 = Relationship::makeRelationship("Modifies", params1);

    SECTION("findRelationship(Relationship rs): empty storage") {
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(res.size() == 0);
    }

    writePkb.setModifiesS(1, {"a", "b", "c"});
    writePkb.setModifiesS(2, {"a"});
    writePkb.setModifiesS(3, {"a", "b"});


    SECTION("findRelationship(Relationship rs): non-empty storage, positive case, LHS: StmtNum, RHS: non-wildcard") {
        std::vector<std::vector<std::string>> expected = {{"1", "a"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs1);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(Relationship rs): non-empty storage, positive case, LHS: StmtNum, RHS: wildcard") {
        // Modifies (1, _)
        std::vector<Parameter> params2 = {Parameter("1", "fixed_int"), Parameter("_", "fixed_string_with_wildcard")};
        Relationship rs2 = Relationship::makeRelationship("Modifies", params2);

        std::vector<std::vector<std::string>> expected = {{"1", "a"}, {"1", "b"}, {"1", "c"}};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(Relationship rs): non-empty storage, positive case, LHS: Stmt, RHS: wildcard") {
        // Modifies (s, _)
        std::vector<Parameter> params2 = {Parameter("s", "stmt"), Parameter("_", "fixed_string_with_wildcard")};
        Relationship rs2 = Relationship::makeRelationship("Modifies", params2);

        std::vector<std::vector<std::string>> expected = {{"1", "a"}, {"1", "b"}, {"1", "c"},
                                                          {"2", "a"}, {"3", "a"}, {"3", "b"}
        };
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }

    SECTION("findRelationship(Relationship rs): non-empty storage, negative case") {
        // Modifies (1, "d")
        std::vector<Parameter> params2 = {Parameter("1", "fixed_int"), Parameter("d", "fixed_string")};
        Relationship rs2 = Relationship::makeRelationship("Modifies", params2);

        std::vector<std::vector<std::string>> expected = {};
        std::vector<std::vector<std::string>> actual = readPkb.findRelationship(rs2);
        std::sort(actual.begin(), actual.end());
        std::sort(expected.begin(), expected.end());
        REQUIRE(actual == expected);
    }
}

TEST_CASE("Checks that writing and reading from ReadPKB works for Follows") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("2", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check==res);
}

TEST_CASE("Checks that a non-existent Follows relationship returns an empty vector from ReadPKB") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("3", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows", params);

    std::vector<std::vector<std::string>> check;
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks that given a followee and a synonym, ReadPKB returns relevant followee-follower pairs") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    writePkb.setStatement("assign", 2);

    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("a", "assign");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks that given a followee and a wildcard, ReadPKB returns all followee-follower pairs") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("_", "wildcard");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks that given a synonym and a follower, ReadPKB returns relevant followee-follower pairs") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    writePkb.setStatement("while", 1);

    Parameter param1 = Parameter("w", "while");
    Parameter param2 = Parameter("2", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks that given a synonym and a synonym, ReadPKB returns relevant followee-follower pairs") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    writePkb.setStatement("while", 1);
    writePkb.setStatement("if", 2);

    Parameter param1 = Parameter("w", "while");
    Parameter param2 = Parameter("if", "if");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks that given a synonym and a wildcard, ReadPKB returns relevant followee-follower pairs") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    writePkb.setStatement("while", 1);
    writePkb.setStatement("assign", 2);

    Parameter param1 = Parameter("w", "while");
    Parameter param2 = Parameter("_", "wildcard");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks that given a wildcard and a follower, ReadPKB returns relevant followee-follower pairs") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);

    Parameter param1 = Parameter("_", "wildcard");
    Parameter param2 = Parameter("2", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks that given a wildcard and a synonym, ReadPKB returns relevant followee-follower pairs") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    writePkb.setStatement("print", 2);

    Parameter param1 = Parameter("_", "wildcard");
    Parameter param2 = Parameter("p", "print");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks that given a wildcard and a wildcard, ReadPKB returns relevant followee-follower pairs") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);

    Parameter param1 = Parameter("_", "wildcard");
    Parameter param2 = Parameter("_", "wildcard");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks that given a followee, if it does not have a certain follower, readPKB returns an empty vector") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    Parameter param1 = Parameter("2", "fixed_int");
    Parameter param2 = Parameter("_", "wildcard");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows", params);

    std::vector<std::vector<std::string>> check;
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks that given a follower, if it does not have a certain followee, readPKB returns an empty vector") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    Parameter param1 = Parameter("_", "wildcard");
    Parameter param2 = Parameter("3", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows", params);

    std::vector<std::vector<std::string>> check;
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
}

TEST_CASE("Checks that PKB pointer in WritePKB and ReadPKB is set to first pkb instance and not overwritten") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb1;
    FollowsStorage fs;
    pkb1.initializePkb();
    writePkb.setInstancePKB(pkb1);
    readPkb.setInstancePKB(pkb1);

    writePkb.setFollows(1, 2);

    PKB pkb2;
    FollowsStorage fs2;
    pkb2.initializePkb();
    writePkb.setInstancePKB(pkb2);

    Parameter param1 = Parameter("1", "fixed_int");
    Parameter param2 = Parameter("2", "fixed_int");
    std::vector<Parameter> params;
    params.push_back(param1);
    params.push_back(param2);
    Relationship rs = Relationship::makeRelationship("Follows", params);

    std::vector<std::vector<std::string>> check = { {"1", "2"} };
    std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs);
    REQUIRE(check == res);

    readPkb.setInstancePKB(pkb2);

    res = readPkb.findRelationship(rs);
    REQUIRE(check == res);
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


