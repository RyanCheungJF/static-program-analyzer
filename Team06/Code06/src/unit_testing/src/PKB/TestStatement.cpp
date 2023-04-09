#include "../../../spa/src/PKB/storage/StmtStorage.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;

TEST_CASE("Check that all statements are recorded in StmtStorage") {

    StmtStorage sts;

    Stmt s = AppConstants::IF;
    sts.writeStatement(s, 3);
    sts.writeStatement(s, 6);
    sts.writeStatement(s, 9);
    std::unordered_set<StmtNum> statementNums = sts.getStatementNumbers(s);
    std::unordered_set<StmtNum> expected = {3, 6, 9};
    REQUIRE(unit_testing_utils::equals(expected, statementNums));
}

TEST_CASE("Check that a statement does not appear in the source code, StmtStorage should return an empty set") {
    StmtStorage sts;

    Stmt s = AppConstants::IF;
    std::unordered_set<StmtNum> statementNums = sts.getStatementNumbers(s);
    REQUIRE(statementNums.empty());
}

TEST_CASE("Check that given query for a statement and a statementNumber that it appears in, it returns true") {
    StmtStorage sts;

    Stmt s = AppConstants::IF;
    sts.writeStatement(s, 3);

    REQUIRE(sts.checkStatement(AppConstants::IF, 3));
    REQUIRE(!sts.checkStatement(AppConstants::IF, 4));
}

TEST_CASE("Check that if a statement does not exist, t returns false") {
    StmtStorage sts;

    REQUIRE(!sts.checkStatement(AppConstants::IF, 4));
}

TEST_CASE("Check that given a Stmt and its StmtNum, a query for an non-existent Stmt returns false") {
    StmtStorage sts;
    REQUIRE(!sts.checkStatement(AppConstants::WHILE, 4));
}

TEST_CASE("StmtStorage: getStatementNumbers(Stmt s): stmt") {
    StmtStorage sts;

    Stmt stmt = AppConstants::STMT;

    SECTION("getStatementNumbers(Stmt s): empty storage") {
        std::unordered_set<StmtNum> statementNums = sts.getStatementNumbers(stmt);
        REQUIRE(statementNums.empty());
    }
    Stmt s1 = AppConstants::IF;
    std::unordered_set<StmtNum> lines1 = {3, 6, 9};
    sts.writeStatement(s1, 3);
    sts.writeStatement(s1, 6);
    sts.writeStatement(s1, 9);

    Stmt s2 = AppConstants::WHILE;
    std::unordered_set<StmtNum> lines2 = {17, 22, 24};
    sts.writeStatement(s2, 17);
    sts.writeStatement(s2, 22);
    sts.writeStatement(s2, 24);

    Stmt s3 = AppConstants::ASSIGN;
    std::unordered_set<StmtNum> lines3 = {5, 7};
    sts.writeStatement(s3, 5);
    sts.writeStatement(s3, 7);

    Stmt s4 = AppConstants::PRINT;
    std::unordered_set<StmtNum> lines4 = {4};
    sts.writeStatement(s4, 4);

    Stmt s5 = AppConstants::READ;
    std::unordered_set<StmtNum> lines5 = {8, 18, 20};
    sts.writeStatement(s5, 8);
    sts.writeStatement(s5, 18);
    sts.writeStatement(s5, 20);

    Stmt s6 = AppConstants::CALL;
    std::unordered_set<StmtNum> lines6 = {23};
    sts.writeStatement(s6, 23);

    SECTION("getStatementNumbers(Stmt s): non-empty storage") {
        std::unordered_set<StmtNum> statementNums = sts.getStatementNumbers(stmt);
        std::unordered_set<StmtNum> expected = {3, 6, 9, 17, 22, 24, 5, 7, 9, 4, 8, 18, 20, 23};
        REQUIRE(unit_testing_utils::equals(expected, statementNums));
    }
}
