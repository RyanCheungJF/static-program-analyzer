#include "catch.hpp"
#include "../../../spa/src/PKB/storage/UsesStorage.h"
<<<<<<< HEAD

TEST_CASE("UsesStorage: writeUsesProcedure, getUsesProcedure") {
    UsesStorage us;
    us.writeUsesProcedure("proc1", {"a", "b", "a", "c"});
    std::unordered_set<Ent> entities1 = us.getUsesProcedure("proc1");
    std::unordered_set<Ent> entities2 = us.getUsesProcedure("proc2");
    std::unordered_set<Ent> entities3 = us.getUsesProcedure("proc");
    bool res = (entities1.size() == 3) && (entities2.size() == 0) && (entities3.size() == 0);
    REQUIRE(res);
}

TEST_CASE("UsesStorage: writeUsesStmtnum, getUsesStmtnum") {
    UsesStorage us;
    us.writeUsesStmtnum(11, {"a", "b", "a", "c"});
    us.writeUsesStmtnum(12, {"a"});
    us.writeUsesStmtnum(13, {});
    std::unordered_set<Ent> entities1 = us.getUsesStmtnum(11);
    std::unordered_set<Ent> entities2 = us.getUsesStmtnum(12);
    std::unordered_set<Ent> entities3 = us.getUsesStmtnum(13);
    std::unordered_set<Ent> entities4 = us.getUsesStmtnum(14);
    bool res = (entities1.size() == 3) && (entities2.size() == 1) && (entities3.size() == 0) && (entities4.size() == 0);
    REQUIRE(res);
}
=======
>>>>>>> pkb-uses-2
