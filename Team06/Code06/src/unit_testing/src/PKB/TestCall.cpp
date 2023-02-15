#include "catch.hpp"
#include "../../../spa/src/PKB/storage/CallStorage.h"


TEST_CASE("CallStorage: writeS") {
    CallStorage cs;

    SECTION("getCallStatements(): empty storage") {

    }

    SECTION("getCallStmt(StmtNum s): empty storage") {

    }

    cs.writeCall(11, "proc2");
    cs.writeCall(21, "proc3");

    SECTION("getCallStatements(): non-empty storage") {

    }

    SECTION("getCallStmt(StmtNum s): non-empty storage") {

    }

}