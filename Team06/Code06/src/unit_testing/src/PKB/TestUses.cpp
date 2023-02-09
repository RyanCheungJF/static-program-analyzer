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

TEST_CASE("UsesStorage: getUsesAllPrintStatementsGivenProcedure") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "input1");
    store.writeUses("proc1", "print", 2, "input2");
    store.writeUses("proc1", "assign", 3, "v");
    store.writeUses("proc1", "assign", 3, "x");
    store.writeUses("proc1", "assign", 3, "5");

    store.writeUses("proc2", "print", 5, "input1");
    store.writeUses("proc2", "print", 6, "input2");
    store.writeUses("proc2", "print", 7, "input2");
    store.writeUses("proc2", "if", 10, "v");
    store.writeUses("proc2", "while", 13, "x");

    std::vector<std::vector<std::string>> result1 = store.getUsesAllPrintStatementsGivenProcedure("proc1");
    std::vector<std::vector<std::string>> result2 = store.getUsesAllPrintStatementsGivenProcedure("proc2");
    bool res = result1.size() == 2 && result2.size() == 3;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllPrintStatementsGivenEntity") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "input1");
    store.writeUses("proc1", "print", 2, "input2");
    store.writeUses("proc1", "print", 2, "input1");
    store.writeUses("proc1", "assign", 3, "v");
    store.writeUses("proc1", "assign", 3, "input1");
    store.writeUses("proc1", "assign", 3, "5");

    std::vector<std::vector<std::string>> result1 = store.getUsesAllPrintStatementsGivenEntity("input1");
    std::vector<std::vector<std::string>> result2 = store.getUsesAllPrintStatementsGivenEntity("input2");
    std::vector<std::vector<std::string>> result3 = store.getUsesAllPrintStatementsGivenEntity("v");
    bool res = result1.size() == 2 && result2.size() == 1 && result3.size() == 0;
    REQUIRE(res);
}
