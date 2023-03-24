#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/utils/utils.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;
using namespace std;

TEST_CASE("Check Pattern Storage Write correctness") {
    PatternStorage ps;
    std::unordered_set<Ent> val1 = {"x", "y"};
    ps.writePattern(1, val1);

    SECTION("Check Writes do not override existing values") {
        std::unordered_set<Ent> temp = {"y"};
        ps.writePattern(2, temp);

        unordered_set<StmtNum> res = ps.getStmtNums("y");

        unordered_set<StmtNum> expected = {1, 2};
        REQUIRE(res.size() == 2);
        REQUIRE(equals(res, expected));
    }

    SECTION("Check that writing existing values do not cause duplicates") {
        std::unordered_set<Ent> temp = {"x", "y"};
        ps.writePattern(1, temp);

        unordered_set<StmtNum> res = ps.getStmtNums("y");

        unordered_set<StmtNum> expected = {1};
        REQUIRE(res.size() == 1);
        REQUIRE(equals(res, expected));
    }
}

TEST_CASE("Check Pattern Storage Write and non-empty Gets") {
    PatternStorage ps;
    std::unordered_set<Ent> temp1 = {"x", "y"};
    std::unordered_set<Ent> temp2 = {"y", "z"};
    std::unordered_set<Ent> temp3 = {"z"};
    ps.writePattern(1, temp1);
    ps.writePattern(2, temp2);
    ps.writePattern(3, temp3);

    SECTION("Get statement numbers of existing pattern") {
        unordered_set<StmtNum> res = ps.getStmtNums("y");
        unordered_set<StmtNum> expected = {1, 2};
        REQUIRE(res.size() == 2);
        REQUIRE(equals(res, expected));
    }

    SECTION("Get all statement number - variable pairs") {
        pair<vector<StmtNum>, vector<Ent>> res = ps.getAllPairs();
        vector<StmtNum> stmtNums = res.first;
        vector<Ent> vars = res.second;

        vector<StmtNum> expectedStmtNums = {1, 1, 2, 2, 3};
        REQUIRE(stmtNums.size() == 5);
        REQUIRE(equals(stmtNums, expectedStmtNums));

        vector<Ent> expectedVars = {"x", "y", "y", "z", "z"};
        REQUIRE(res.second.size() == 5);
        REQUIRE(equals(vars, expectedVars));
    }
}

TEST_CASE("Check Pattern Storage Write and empty Gets") {
    PatternStorage ps;

    std::unordered_set<Ent> temp1 = {"x", "y"};
    std::unordered_set<Ent> temp2 = {"y", "z"};
    std::unordered_set<Ent> temp3 = {"z"};
    ps.writePattern(1, temp1);
    ps.writePattern(2, temp2);
    ps.writePattern(3, temp3);

    SECTION("Get statement numbers of non-existing pattern") {
        unordered_set<StmtNum> res = ps.getStmtNums("non-existing");
        REQUIRE(res.size() == 0);
    }
}

TEST_CASE("Test If/While Pattern on WritePKB and ReadPKB") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);


    std::unordered_set<Ent> temp1 = {"x", "y", "z"};
    std::unordered_set<Ent> temp2 = {"z"};

    writePkb.setIfPattern(1, temp1);
    writePkb.setIfPattern(2, temp2);

    writePkb.setWhilePattern(1, temp1);
    writePkb.setWhilePattern(2, temp2);

    SECTION("if(\"x\", _, _)") {
        Parameter param1 = Parameter("if", ParameterType::IF);
        Parameter param2 = Parameter("z", ParameterType::FIXED_STRING);
        vector<string> exprSpecs = {"_", "_"};
        Pattern pattern = Pattern(param1, param2, exprSpecs);

        std::vector<std::vector<std::string>> res = readPkb.findPattern(pattern);
        std::vector<std::vector<std::string>> expected = {{"1", "z"}, {"2", "z"}};
        REQUIRE(res.size() == 2);
        REQUIRE(equals(res, expected));
    }

    SECTION("while(v, _)") {
        Parameter param1 = Parameter("while", ParameterType::WHILE);
        Parameter param2 = Parameter("v", ParameterType::VARIABLE);
        vector<string> exprSpecs = {"_", "_"};
        Pattern pattern = Pattern(param1, param2, exprSpecs);

        std::vector<std::vector<std::string>> res = readPkb.findPattern(pattern);
        std::vector<std::vector<std::string>> expected = {{"1", "x"}, {"1", "y"}, {"1", "z"}, {"2", "z"}};
        REQUIRE(res.size() == 4);
        REQUIRE(equals(res, expected));
    }

    SECTION("if(_, _, _)") {
        Parameter param1 = Parameter("if", ParameterType::IF);
        Parameter param2 = Parameter("_", ParameterType::WILDCARD);
        vector<string> exprSpecs = {"_", "_"};
        Pattern pattern = Pattern(param1, param2, exprSpecs);

        std::vector<std::vector<std::string>> res = readPkb.findPattern(pattern);
        std::vector<std::vector<std::string>> expected = {{"1", "x"}, {"1", "y"}, {"1", "z"}, {"2", "z"}};
        REQUIRE(res.size() == 4);
        REQUIRE(equals(res, expected));
    }

    SECTION("while(\"non-exist\", _, _)") {
        Parameter param1 = Parameter("while", ParameterType::WHILE);
        Parameter param2 = Parameter("non-exist", ParameterType::FIXED_STRING);
        vector<string> exprSpecs = {"_", "_"};
        Pattern pattern = Pattern(param1, param2, exprSpecs);

        std::vector<std::vector<std::string>> res = readPkb.findPattern(pattern);
        REQUIRE(res.size() == 0);
    }
}
