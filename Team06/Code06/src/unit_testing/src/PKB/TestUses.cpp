#include "catch.hpp"
#include "../../../spa/src/PKB/storage/UsesStorage.h"

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

TEST_CASE("UsesStorage: getUsesAllAssignStatements") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "input1");
    store.writeUses("proc1", "print", 2, "input2");
    store.writeUses("proc1", "assign", 3, "v");
    store.writeUses("proc1", "assign", 3, "x");
    store.writeUses("proc1", "assign", 3, "5");

    std::vector<std::vector<std::string>> result = store.getUsesAllAssignStatements();
    bool res = result.size() == 3;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllAssignStatementsGivenProcedure") {
    UsesStorage store;
    store.writeUses("proc1", "assign", 3, "v");
    store.writeUses("proc1", "assign", 3, "x");
    store.writeUses("proc1", "assign", 3, "5");

    store.writeUses("proc2", "assign", 5, "input1");
    store.writeUses("proc2", "assign", 5, "input2");
    store.writeUses("proc2", "print", 7, "input2");
    store.writeUses("proc2", "if", 10, "v");
    store.writeUses("proc2", "while", 13, "x");

    std::vector<std::vector<std::string>> result1 = store.getUsesAllAssignStatementsGivenProcedure("proc1");
    std::vector<std::vector<std::string>> result2 = store.getUsesAllAssignStatementsGivenProcedure("proc2");
    bool res = result1.size() == 3 && result2.size() == 2;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllAssignStatementsGivenEntity") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "input1");
    store.writeUses("proc1", "print", 2, "input2");
    store.writeUses("proc1", "print", 2, "input1");
    store.writeUses("proc1", "assign", 3, "v");
    store.writeUses("proc1", "assign", 3, "input1");
    store.writeUses("proc1", "assign", 3, "5");

    store.writeUses("proc2", "assign", 6, "input1");
    store.writeUses("proc2", "assign", 6, "5");

    std::vector<std::vector<std::string>> result1 = store.getUsesAllAssignStatementsGivenEntity("input1");
    std::vector<std::vector<std::string>> result2 = store.getUsesAllAssignStatementsGivenEntity("input2");
    std::vector<std::vector<std::string>> result3 = store.getUsesAllAssignStatementsGivenEntity("v");
    std::vector<std::vector<std::string>> result4 = store.getUsesAllAssignStatementsGivenEntity("5");
    bool res = result1.size() == 2 && result2.size() == 0 && result3.size() == 1 && result4.size() == 2;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllIfStatements") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "input1");
    store.writeUses("proc1", "print", 2, "input2");
    store.writeUses("proc1", "assign", 3, "v");
    store.writeUses("proc1", "assign", 3, "x");
    store.writeUses("proc1", "assign", 3, "5");

    std::vector<std::vector<std::string>> result = store.getUsesAllIfStatements();
    bool res = result.size() == 0;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllIfStatementsGivenProcedure") {
    UsesStorage store;
    store.writeUses("proc1", "assign", 3, "v");
    store.writeUses("proc1", "assign", 3, "x");
    store.writeUses("proc1", "assign", 3, "5");

    store.writeUses("proc2", "assign", 5, "input1");
    store.writeUses("proc2", "assign", 5, "input2");
    store.writeUses("proc2", "print", 7, "input2");
    store.writeUses("proc2", "if", 10, "v");
    store.writeUses("proc2", "while", 13, "x");

    std::vector<std::vector<std::string>> result1 = store.getUsesAllIfStatementsGivenProcedure("proc1");
    std::vector<std::vector<std::string>> result2 = store.getUsesAllIfStatementsGivenProcedure("proc2");
    bool res = result1.size() == 0 && result2.size() == 1;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllIfStatementsGivenEntity") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "input1");
    store.writeUses("proc1", "if", 3, "v");
    store.writeUses("proc1", "if", 3, "5");

    store.writeUses("proc2", "if", 6, "input1");
    store.writeUses("proc2", "if", 6, "5");

    std::vector<std::vector<std::string>> result1 = store.getUsesAllIfStatementsGivenEntity("input1");
    std::vector<std::vector<std::string>> result2 = store.getUsesAllIfStatementsGivenEntity("input2");
    std::vector<std::vector<std::string>> result3 = store.getUsesAllIfStatementsGivenEntity("v");
    std::vector<std::vector<std::string>> result4 = store.getUsesAllIfStatementsGivenEntity("5");
    bool res = result1.size() == 1 && result2.size() == 0 && result3.size() == 1 && result4.size() == 2;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllWhileStatements") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "input1");
    store.writeUses("proc1", "print", 2, "input2");
    store.writeUses("proc1", "while", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "while", 6, "x");

    std::vector<std::vector<std::string>> result = store.getUsesAllWhileStatements();
    bool res = result.size() == 3;
    REQUIRE(res);
}


TEST_CASE("UsesStorage: getUsesAllWhileStatementsGivenProcedure") {
    UsesStorage store;
    store.writeUses("proc1", "assign", 2, "v");
    store.writeUses("proc1", "while", 3, "x");
    store.writeUses("proc1", "while", 3, "5");

    store.writeUses("proc2", "assign", 5, "input1");
    store.writeUses("proc2", "assign", 5, "input2");
    store.writeUses("proc2", "print", 7, "input2");
    store.writeUses("proc2", "if", 10, "v");
    store.writeUses("proc2", "while", 13, "x");

    std::vector<std::vector<std::string>> result1 = store.getUsesAllWhileStatementsGivenProcedure("proc1");
    std::vector<std::vector<std::string>> result2 = store.getUsesAllWhileStatementsGivenProcedure("proc2");
    bool res = result1.size() == 2 && result2.size() == 1;
    REQUIRE(res);
}


TEST_CASE("UsesStorage: getUsesAllWhileStatementsGivenEntity") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "input1");
    store.writeUses("proc1", "if", 3, "v");
    store.writeUses("proc1", "while", 4, "5");
    store.writeUses("proc1", "while", 4, "v");

    store.writeUses("proc2", "while", 6, "input1");
    store.writeUses("proc2", "while", 6, "5");

    std::vector<std::vector<std::string>> result1 = store.getUsesAllWhileStatementsGivenEntity("input1");
    std::vector<std::vector<std::string>> result2 = store.getUsesAllWhileStatementsGivenEntity("input2");
    std::vector<std::vector<std::string>> result3 = store.getUsesAllWhileStatementsGivenEntity("v");
    std::vector<std::vector<std::string>> result4 = store.getUsesAllWhileStatementsGivenEntity("5");
    bool res = result1.size() == 1 && result2.size() == 0 && result3.size() == 1 && result4.size() == 2;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllCallStatements_format1 no loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "v");
//    store.writeUsesCall("proc3", "proc1", 11);

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatements_format1();
    std::cout << "PRINTING\n";
    //Expected: [8, x], [8, v], [2, 1 (constant)], [2, v], [2, b], [2, x]
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 6;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllCallStatements_format1 with loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "v");
    store.writeUsesCall("proc3", "proc1", 11);

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatements_format1();
    std::cout << "PRINTING\n";
    //Expected: [2, a], [2, 1], [2, v], [2, b], [2, x]... (ie * 3 since we got 3 procs)
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 15;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllCallStatements_format2 no loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "v");

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatements_format2();
    std::cout << "PRINTING\n";
    //Expected: [8, proc2], [2, proc1]
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 2;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllCallStatements_format2 with loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "v");
    store.writeUsesCall("proc3", "proc1", 11);

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatements_format2();
    std::cout << "PRINTING\n";
    //Expected: [8, proc2], [2, proc1], [11, proc3]
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 3;
    REQUIRE(res);
}


TEST_CASE("UsesStorage: getUsesAllCallStatementsGivenProcedure_format1 no loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "v");
//    store.writeUsesCall("proc3", "proc1", 11);

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatementsGivenProcedure_format1("proc1");
    std::cout << "PRINTING\n";
    //Expected: [2, 1 (constant)], [2, v], [2, b], [2, x]
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 4;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllCallStatementsGivenProcedure_format1 with loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "v");
    store.writeUsesCall("proc3", "proc1", 11);

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatementsGivenProcedure_format1("proc1");
    std::cout << "PRINTING\n";
    //Expected: [2, 1 (constant)], [2, v], [2, b], [2, x], [2, a]
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 5;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllCallStatementsGivenProcedure_format2 no loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "v");
//    store.writeUsesCall("proc3", "proc1", 11);

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatementsGivenProcedure_format2("proc1");
    std::cout << "PRINTING\n";
    //Expected: [2, proc2]
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 1;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllCallStatementsGivenProcedure_format2 with loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "v");
    store.writeUsesCall("proc3", "proc1", 11);

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatementsGivenProcedure_format2("proc1");
    std::cout << "PRINTING\n";
    //Expected: [2, proc2]
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 1;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllCallStatementsGivenProcedure_format3 no loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "v");
//    store.writeUsesCall("proc3", "proc1", 11);

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatementsGivenProcedure_format3("proc1");
    std::cout << "PRINTING\n";
    //Expected: [2, proc3], [2, proc3]
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 2;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllCallStatementsGivenProcedure_format3 with loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "v");
    store.writeUsesCall("proc3", "proc1", 11);

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatementsGivenProcedure_format3("proc1");
    std::cout << "PRINTING\n";
    //Expected: [2, proc3], [2, proc3], [2, proc1]
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 3;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllCallStatementsGivenEntity_format1 no loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "a");
//    store.writeUsesCall("proc3", "proc1", 11);

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatementsGivenEntity_format1("a");
    std::cout << "PRINTING\n";
    //Expected: [8, a], [2, a]
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 2;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllCallStatementsGivenEntity_format1 with loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "a");
    store.writeUsesCall("proc3", "proc1", 11);

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatementsGivenEntity_format1("a");
    std::cout << "PRINTING\n";
    //Expected: [8, a], [2, a], [11, a]
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 3;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllCallStatementsGivenEntity_format2 no loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "a");
//    store.writeUsesCall("proc3", "proc1", 11);

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatementsGivenEntity_format2("a");
    std::cout << "PRINTING\n";
    //Expected: [8, proc3], [2, proc2]
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 2;
    REQUIRE(res);
}

TEST_CASE("UsesStorage: getUsesAllCallStatementsGivenEntity_format2 with loop") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "a");
    store.writeUsesCall("proc1", "proc2", 2);
    store.writeUses("proc2", "assign", 3, "1");
    store.writeUses("proc2", "while", 6, "v");
    store.writeUses("proc2", "assign", 7, "b");
    store.writeUsesCall("proc2", "proc3", 8);
    store.writeUses("proc3", "assign", 9, "x");
    store.writeUses("proc3", "assign", 10, "a");
    store.writeUsesCall("proc3", "proc1", 11);

    std::vector<std::vector<std::string>> result = store.getUsesAllCallStatementsGivenEntity_format2("a");
    std::cout << "PRINTING\n";
    //Expected: [8, proc3], [2, proc2], [11, proc1]
//    for (auto i : result) {
//        std::cout << i[0] << " " << i[1] << "\n";
//    }
    bool res = result.size() == 3;
    REQUIRE(res);
}

/*
TEST_CASE("UsesStorage: getUsesAllWhileStatementsGivenProcedure") {
    UsesStorage store;
    store.writeUses("proc1", "assign", 2, "v");
    store.writeUses("proc1", "while", 3, "x");
    store.writeUses("proc1", "while", 3, "5");

    store.writeUses("proc2", "assign", 5, "input1");
    store.writeUses("proc2", "assign", 5, "input2");
    store.writeUses("proc2", "print", 7, "input2");
    store.writeUses("proc2", "if", 10, "v");
    store.writeUses("proc2", "while", 13, "x");

    std::vector<std::vector<std::string>> result1 = store.getUsesAllWhileStatementsGivenProcedure("proc1");
    std::vector<std::vector<std::string>> result2 = store.getUsesAllWhileStatementsGivenProcedure("proc2");
    bool res = result1.size() == 2 && result2.size() == 1;
    REQUIRE(res);
}


TEST_CASE("UsesStorage: getUsesAllWhileStatementsGivenEntity") {
    UsesStorage store;
    store.writeUses("proc1", "print", 1, "input1");
    store.writeUses("proc1", "if", 3, "v");
    store.writeUses("proc1", "while", 4, "5");
    store.writeUses("proc1", "while", 4, "v");

    store.writeUses("proc2", "while", 6, "input1");
    store.writeUses("proc2", "while", 6, "5");


    std::vector<std::vector<std::string>> result1 = store.getUsesAllWhileStatementsGivenEntity("input1");
    std::vector<std::vector<std::string>> result2 = store.getUsesAllWhileStatementsGivenEntity("input2");
    std::vector<std::vector<std::string>> result3 = store.getUsesAllWhileStatementsGivenEntity("v");
    std::vector<std::vector<std::string>> result4 = store.getUsesAllWhileStatementsGivenEntity("5");
    bool res = result1.size() == 1 && result2.size() == 0 && result3.size() == 1 && result4.size() == 2;
    REQUIRE(res);
}
*/

