#include "../../../spa/src/PKB/storage/CallStorage.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;

TEST_CASE("CallStorage: writeS") {
    CallStorage cs;
    StmtNum s = 10;

    SECTION("getCallStmt(StmtNum s): empty storage") {
        std::pair<StmtNum, ProcName> res = cs.getCallStmt(s);
        REQUIRE(res.first == AppConstants::NOT_USED_FIELD);
        REQUIRE(res.second == AppConstants::PROCEDURE_DOES_NOT_EXIST);
    }

    ProcName proc1 = "proc1";
    ProcName proc2 = "proc2";

    cs.writeCallS(11, proc1);
    cs.writeCallS(21, proc2);

    SECTION("getCallStmt(StmtNum s): non-empty storage") {
        auto i = cs.getCallStmt(11);
        REQUIRE(i.first == 11);
        REQUIRE(i.second == proc1);

        REQUIRE(!(i.first == 12));
        REQUIRE(!(i.second == proc2));
    }
}
