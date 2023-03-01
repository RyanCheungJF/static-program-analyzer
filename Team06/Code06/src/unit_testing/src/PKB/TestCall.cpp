#include "../../../spa/src/PKB/storage/CallStorage.h"
#include "catch.hpp"

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

  cs.writeCall(11, proc1);
  cs.writeCall(21, proc2);

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