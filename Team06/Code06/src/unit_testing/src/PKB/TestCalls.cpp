#include "../../../spa/src/PKB/storage/CallsStorage.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;

TEST_CASE("CallsStorage: writeP") {
   CallsStorage cs;
   ProcName caller = "proc0";

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

        std::unordered_set<ProcName> res3 = cs.getCallers(caller);
        REQUIRE(unit_testing_utils::equals({}, res3));

        std::unordered_set<ProcName> res4 = cs.getCallers(proc1);
        REQUIRE(unit_testing_utils::equals({caller}, res4));
   }
}
