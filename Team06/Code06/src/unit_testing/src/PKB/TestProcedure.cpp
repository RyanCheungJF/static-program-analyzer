#include "catch.hpp"
#include "../../../spa/src/PKB/storage/ProcedureStorage.h"
#include "../utils/utils.h"

using namespace unit_testing_utils;


TEST_CASE("All statement numbers are recorded in their respective procedures") {
    ProcedureStorage store;
    ProcName p = "calculateEuclidean";
    std::unordered_set<StmtNum> lines = {2, 3, 4};
    store.writeProcedure(p, lines);
    std::unordered_set<StmtNum> statementNums = store.getProcedureStatementNumbers(p);
    REQUIRE(unit_testing_utils::equals(lines, statementNums));
}

TEST_CASE("If a procedure does not appear in the source code, getProcedureStatementNumbers() should return an empty set") {
    ProcedureStorage store;
    ProcName p = "calculateEuclidean";
    std::unordered_set<StmtNum> statementNums = store.getProcedureStatementNumbers(p);
    REQUIRE(statementNums.empty());
}

TEST_CASE("Given a query for a procedure and a statementNumber that is not nested in the procedure, checkProcedure() returns false") {
    ProcedureStorage store;
    ProcName p = "computeCentroid";
    std::unordered_set<StmtNum> lines = {2};
    store.writeProcedure(p, lines);
    REQUIRE(!store.checkProcedure(p, 4));
}

TEST_CASE("If a procedure does not exist, checkProcedure() returns false") {
    ProcedureStorage store;
    REQUIRE(!store.checkProcedure("procedureThatDoesNotExist", 4));
}
