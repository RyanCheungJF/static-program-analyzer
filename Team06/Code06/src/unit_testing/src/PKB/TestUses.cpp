#include "catch.hpp"
#include "../../../spa/src/PKB/storage/UsesStorage.h"

using namespace std;

TEST_CASE("UsesStorage: getUsesAllPrintStatements") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "input1");
    store.writeUses("proc1", "print", 2, "input2");
    store.writeUses("proc1", "assign", 3, "v");
    store.writeUses("proc1", "assign", 3, "x");
    store.writeUses("proc1", "assign", 3, "5");

    std::vector<std::vector<std::string>> result = store.getUsesAllPrintStatements();
    bool res = result.size() == 2;
    REQUIRE(res);
}