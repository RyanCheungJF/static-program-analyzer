#include "catch.hpp"
#include "../../../spa/src/PKB/storage/ModifiesStorage.h"

TEST_CASE("ModifiesStorage: writeModifiesProcedure, getModifiesProcedure") {
    ModifiesStorage us;
    us.writeModifiesProcedure("proc1", {"a", "b", "a", "c"});
    std::unordered_set<Ent> entities1 = us.getModifiesProcedure("proc1");
    std::unordered_set<Ent> entities2 = us.getModifiesProcedure("proc2");
    std::unordered_set<Ent> entities3 = us.getModifiesProcedure("proc");
    bool res = (entities1.size() == 3) && (entities2.size() == 0) && (entities3.size() == 0);
    REQUIRE(res);
}

TEST_CASE("ModifiesStorage: writeModifiesStmtnum, getModifiesStmtnum") {
    ModifiesStorage us;
    us.writeModifiesStmtnum(11, {"a", "b", "a", "c"});
    us.writeModifiesStmtnum(12, {"a"});
    us.writeModifiesStmtnum(13, {});
    std::unordered_set<Ent> entities1 = us.getModifiesStmtnum(11);
    std::unordered_set<Ent> entities2 = us.getModifiesStmtnum(12);
    std::unordered_set<Ent> entities3 = us.getModifiesStmtnum(13);
    std::unordered_set<Ent> entities4 = us.getModifiesStmtnum(14);
    bool res = (entities1.size() == 3) && (entities2.size() == 1) && (entities3.size() == 0) && (entities4.size() == 0);
    REQUIRE(res);
}


