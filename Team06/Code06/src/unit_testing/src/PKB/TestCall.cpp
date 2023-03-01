#include "../../../spa/src/PKB/storage/CallStorage.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;

TEST_CASE("CallStorage: writeS") {
  CallStorage cs;
  StmtNum s = 10;

  SECTION("getCallStmt(StmtNum s): empty storage") {
    std::pair<StmtNum, ProcName> res = cs.getCallStmt(s);
    REQUIRE(res.first == -1);
    REQUIRE(res.second == "INVALID");
  }

  SECTION("getCallStatements(): empty storage") {
    auto res = cs.getCallStatements();
    REQUIRE(res.empty());
  }

  ProcName proc1 = "proc1";
  ProcName proc2 = "proc2";

  cs.writeCallS(11, proc1);
  cs.writeCallS(21, proc2);

  SECTION("getCallStatements(): non-empty storage") {
    auto i = cs.getCallStatements();
    REQUIRE(i.size() == 2);
  }

  SECTION("getCallStmt(StmtNum s): non-empty storage") {
    auto i = cs.getCallStmt(11);
    REQUIRE(i.first == 11);
    REQUIRE(i.second == proc1);

    REQUIRE(!(i.first == 12));
    REQUIRE(!(i.second == proc2));
  }
}

TEST_CASE("CallStorage: writeP") {
    CallStorage cs;
    ProcName caller = "proc1";

    SECTION("getCallees(procName): empty storage") {
        std::unordered_set<ProcName> res = cs.getCallees(caller);
        REQUIRE(res.empty());
    }

    ProcName proc1 = "proc1";
    ProcName proc2 = "proc2";

    std::unordered_set<ProcName> callees = {proc1, proc2};
    cs.writeCallP(caller, callees);

    SECTION("getCallees(procName): non-empty storage") {
        std::unordered_set<ProcName> res1 = cs.getCallees("nonExistent");
        REQUIRE(res1.empty());

        std::unordered_set<ProcName> res2 = cs.getCallees(caller);
        REQUIRE(unit_testing_utils::equals({"proc1", "proc2"}, res2));
    }
}
