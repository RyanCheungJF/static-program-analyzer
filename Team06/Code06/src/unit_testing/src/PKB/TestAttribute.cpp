#include "../../../spa/src/PKB/PKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace std;
using namespace unit_testing_utils;

TEST_CASE("Select attributes") {
    PKB pkb;
    pkb.initializePkb();

    pkb.setStatement(AppConstants::CALL, 1);
    pkb.setCall(1, "main");

    pkb.setStatement(AppConstants::READ, 2);
    std::unordered_set<Ent> ents = {"x"};
    pkb.setEntity(2, ents);

    pkb.setStatement(AppConstants::PRINT, 3);
    ents = {"y"};
    pkb.setEntity(3, ents);

    pkb.setStatement(AppConstants::WHILE, 4);

    std::unordered_set<Const> constants = {"1", "2"};
    pkb.setConstant(5, constants);

    std::unordered_set<StmtNum> procLines = {1, 2, 3, 4, 5};
    pkb.setProcedure("main", procLines);

    SECTION("Select call.procName") {
        Parameter param = Parameter("c", ParameterType::CALL, AttributeType::PROCNAME);
        vector<vector<string>> res = pkb.findAttribute(param);
        vector<vector<string>> expected = {{"1", "main"}};
        REQUIRE(res == expected);
    }

    SECTION("Select read.varName") {
        Parameter param = Parameter("r", ParameterType::READ, AttributeType::VARNAME);
        vector<vector<string>> res = pkb.findAttribute(param);
        vector<vector<string>> expected = {{"2", "x"}};
        REQUIRE(res == expected);
    }

    SECTION("Select print.varName") {
        Parameter param = Parameter("p", ParameterType::PRINT, AttributeType::VARNAME);
        vector<vector<string>> res = pkb.findAttribute(param);
        vector<vector<string>> expected = {{"3", "y"}};
        REQUIRE(res == expected);
    }

    SECTION("Select while.stmt#") {
        Parameter param = Parameter("w", ParameterType::WHILE, AttributeType::STMTNO);
        vector<vector<string>> res = pkb.findAttribute(param);
        vector<vector<string>> expected = {{"4", "4"}};
        REQUIRE(res == expected);
    }

    SECTION("Select var.varName") {
        Parameter param = Parameter("v", ParameterType::VARIABLE, AttributeType::VARNAME);
        vector<vector<string>> res = pkb.findAttribute(param);
        vector<vector<string>> expected = {{"x", "x"}, {"y", "y"}};
        REQUIRE(equals(expected, res));
    }

    SECTION("Select const.value") {
        Parameter param = Parameter("c", ParameterType::CONSTANT, AttributeType::VALUE);
        vector<vector<string>> res = pkb.findAttribute(param);
        vector<vector<string>> expected = {{"1", "1"}, {"2", "2"}};
        REQUIRE(equals(expected, res));
    }

    SECTION("Select proc.procName") {
        Parameter param = Parameter("p", ParameterType::PROCEDURE, AttributeType::PROCNAME);
        vector<vector<string>> res = pkb.findAttribute(param);
        vector<vector<string>> expected = {{"main", "main"}};
        REQUIRE(res == expected);
    }
}