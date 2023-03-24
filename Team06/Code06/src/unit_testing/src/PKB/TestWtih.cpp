#include "../../../spa/src/PKB/PKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace std;
using namespace unit_testing_utils;

TEST_CASE("Test findWith()") {
    PKB pkb;
    pkb.initializePkb();

    pkb.setStatement(AppConstants::CALL, 1);
    pkb.setStatement(AppConstants::STMT, 1);
    pkb.setCall(1, "sub");

    pkb.setStatement(AppConstants::READ, 2);
    pkb.setStatement(AppConstants::STMT, 2);
    std::unordered_set<Ent> ents = {"x"};
    pkb.setEntity(2, ents);

    pkb.setStatement(AppConstants::PRINT, 3);
    pkb.setStatement(AppConstants::STMT, 3);
    ents = {"sub"};
    pkb.setEntity(3, ents);

    pkb.setStatement(AppConstants::WHILE, 4);
    pkb.setStatement(AppConstants::STMT, 4);

    pkb.setStatement(AppConstants::ASSIGN, 5);
    pkb.setStatement(AppConstants::STMT, 5);
    std::unordered_set<Const> constants = {"1", "2"};
    pkb.setConstant(5, constants);

    std::unordered_set<StmtNum> procLines = {1, 2, 3, 4, 5};
    pkb.setProcedure("main", procLines);

    pkb.setStatement(AppConstants::PRINT, 6);
    pkb.setStatement(AppConstants::STMT, 6);
    ents = {"main"};
    pkb.setEntity(6, ents);

    procLines = {6};
    pkb.setProcedure("sub", procLines);

    SECTION("Attribute equals fixed value") {
        SECTION("with stmt.stmt# = \"2\"") {
            Parameter leftParam = Parameter("s", ParameterType::STMT, AttributeType::STMTNO);
            Parameter rightParam = Parameter("2", ParameterType::FIXED_INT);
            Comparison comp = Comparison(ComparisonOperator::EQUALS, leftParam, rightParam);
            vector<vector<string>> res = pkb.findWith(comp);
            vector<vector<string>> expected = {{"2", "2"}};
            REQUIRE(res == expected);
        }

        SECTION("with call.procName# = \"main\"") {
            Parameter leftParam = Parameter("c", ParameterType::CALL, AttributeType::PROCNAME);
            Parameter rightParam = Parameter("sub", ParameterType::FIXED_STRING);
            Comparison comp = Comparison(ComparisonOperator::EQUALS, leftParam, rightParam);
            vector<vector<string>> res = pkb.findWith(comp);
            vector<vector<string>> expected = {{"1", "sub"}};
            REQUIRE(res == expected);
        }

        SECTION("with read.varName# = \"x\"") {
            Parameter leftParam = Parameter("r", ParameterType::READ, AttributeType::VARNAME);
            Parameter rightParam = Parameter("x", ParameterType::FIXED_STRING);
            Comparison comp = Comparison(ComparisonOperator::EQUALS, leftParam, rightParam);
            vector<vector<string>> res = pkb.findWith(comp);
            vector<vector<string>> expected = {{"2", "x"}};
            REQUIRE(res == expected);
        }

        SECTION("with print.varName# = \"x\"") {
            Parameter leftParam = Parameter("p", ParameterType::PRINT, AttributeType::VARNAME);
            Parameter rightParam = Parameter("x", ParameterType::FIXED_STRING);
            Comparison comp = Comparison(ComparisonOperator::EQUALS, leftParam, rightParam);
            vector<vector<string>> res = pkb.findWith(comp);
            vector<vector<string>> expected = {};
            REQUIRE(res == expected);
        }
    }

    SECTION("Attribute equals another attribute") {
        SECTION("with stmt.stmt# = const.value") {
            Parameter leftParam = Parameter("s", ParameterType::STMT, AttributeType::STMTNO);
            Parameter rightParam = Parameter("c", ParameterType::CONSTANT, AttributeType::VALUE);
            Comparison comp = Comparison(ComparisonOperator::EQUALS, leftParam, rightParam);
            vector<vector<string>> res = pkb.findWith(comp);
            vector<vector<string>> expected = {{"1", "1"}, {"2", "2"}};
            REQUIRE(res == expected);
        }

        SECTION("with call.procName# = print.varName") {
            Parameter leftParam = Parameter("c", ParameterType::CALL, AttributeType::PROCNAME);
            Parameter rightParam = Parameter("p", ParameterType::PRINT, AttributeType::VARNAME);
            Comparison comp = Comparison(ComparisonOperator::EQUALS, leftParam, rightParam);
            vector<vector<string>> res = pkb.findWith(comp);
            vector<vector<string>> expected = {{"1", "3"}};
            REQUIRE(res == expected);
        }

        SECTION("with print.varName = proc.procName") {
            Parameter leftParam = Parameter("p", ParameterType::PRINT, AttributeType::VARNAME);
            Parameter rightParam = Parameter("p", ParameterType::PROCEDURE, AttributeType::PROCNAME);
            Comparison comp = Comparison(ComparisonOperator::EQUALS, leftParam, rightParam);
            vector<vector<string>> res = pkb.findWith(comp);
            vector<vector<string>> expected = {{"3", "sub"}, {"6", "main"}};
            REQUIRE(equals(res, expected));
        }

        SECTION("with read.varName = print.varName") {
            Parameter leftParam = Parameter("r", ParameterType::READ, AttributeType::VARNAME);
            Parameter rightParam = Parameter("p", ParameterType::PRINT, AttributeType::VARNAME);
            Comparison comp = Comparison(ComparisonOperator::EQUALS, leftParam, rightParam);
            vector<vector<string>> res = pkb.findWith(comp);
            vector<vector<string>> expected = {};
            REQUIRE(res == expected);
        }

        SECTION("with var.varName = read.stmt#") {
            Parameter leftParam = Parameter("p", ParameterType::VARIABLE, AttributeType::VARNAME);
            Parameter rightParam = Parameter("r", ParameterType::READ, AttributeType::STMTNO);
            Comparison comp = Comparison(ComparisonOperator::EQUALS, leftParam, rightParam);
            vector<vector<string>> res = pkb.findWith(comp);
            vector<vector<string>> expected = {};
            REQUIRE(res == expected);
        }
    }
}
