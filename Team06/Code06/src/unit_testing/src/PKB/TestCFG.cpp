#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;

TEST_CASE("CFGStorage, ReadPKB, WritePKB, no while loop") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    SECTION("CFGStorage: procedure does not exist") {
        StmtNum num = 1;
        CFGNodeStub node;
        writePkb.writeCFG(num, node);
        REQUIRE(readPkb.getCFG(2) == nullptr);
    }

    SECTION("CFGStorage: empty procedure") {
        StmtNum num = 1;
        CFGNodeStub node;
        writePkb.writeCFG(num, node);

        auto root = readPkb.getCFG(num);
        auto children = root->children;

        REQUIRE(children.empty());
    }

    CFGNodeStub root;
    root.first = 1;
    root.last = 2;
    CFGNodeStub node1;
    node1.first = 3;
    node1.last = 4;
    CFGNodeStub node2;
    node2.first = 5;
    node2.last = 6;
    CFGNodeStub node3;
    node3.first = 7;
    node3.last = 9;
    CFGNodeStub node4;
    node4.first = 10;
    node4.last = 12;
    CFGNodeStub node5;
    node5.first = 13;
    node5.last = 13;

    node5.children = {};
    node4.children = {&node5};
    node3.children = {&node4};
    node2.children = {&node4};
    node1.children = {&node3, &node2};
    root.children = {&node1};

    root.parents = {};
    node1.parents = {&root};
    node2.parents = {&node1};
    node3.parents = {&node1};
    node4.parents = {&node2, &node3};
    node5.parents = {&node4};

    writePkb.writeCFG(1, root);
    writePkb.writeCFG(2, root);
    writePkb.writeCFG(3, node1);
    writePkb.writeCFG(4, node1);
    writePkb.writeCFG(5, node2);
    writePkb.writeCFG(6, node2);
    writePkb.writeCFG(7, node3);
    writePkb.writeCFG(8, node3);
    writePkb.writeCFG(9, node3);
    writePkb.writeCFG(10, node4);
    writePkb.writeCFG(11, node4);
    writePkb.writeCFG(12, node4);
    writePkb.writeCFG(13, node5);

    SECTION("CFGStorage: Next(n1, n2) given n1 find n2") {
        std::vector<StmtNum> res1 = readPkb.getNextRHS(3);
        std::vector<StmtNum> expected1 = {4};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<StmtNum> res2 = readPkb.getNextRHS(4);
        std::vector<StmtNum> expected2 = {5, 7};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<StmtNum> res3 = readPkb.getNextRHS(13);
        std::vector<StmtNum> expected3 = {};
        REQUIRE(unit_testing_utils::equals(expected3, res3));
    }

    SECTION("CFGStorage: Next(n1, n2) given n2 find n1") {
        std::vector<StmtNum> res1 = readPkb.getNextLHS(5);
        std::vector<StmtNum> expected1 = {4};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<StmtNum> res2 = readPkb.getNextLHS(7);
        std::vector<StmtNum> expected2 = {4};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<StmtNum> res3 = readPkb.getNextLHS(4);
        std::vector<StmtNum> expected3 = {3};
        REQUIRE(unit_testing_utils::equals(expected3, res3));

        std::vector<StmtNum> res4 = readPkb.getNextLHS(10);
        std::vector<StmtNum> expected4 = {6, 9};
        REQUIRE(unit_testing_utils::equals(expected4, res4));

        std::vector<StmtNum> res5 = readPkb.getNextLHS(1);
        std::vector<StmtNum> expected5 = {};
        REQUIRE(unit_testing_utils::equals(expected5, res5));
    }
}

TEST_CASE("CFGStorage, ReadPKB, WritePKB, have while loop") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    CFGNodeStub root;
    root.first = 1;
    root.last = 2;
    CFGNodeStub node1;
    node1.first = 3;
    node1.last = 4;
    CFGNodeStub node2;
    node2.first = 5;
    node2.last = 6;
    CFGNodeStub node3;
    node3.first = 7;
    node3.last = 9;
    CFGNodeStub node4;
    node4.first = 10;
    node4.last = 12;
    CFGNodeStub node5;
    node5.first = 13;
    node5.last = 13;

    node5.children = {};
    node4.children = {&node5, &node1}; // introduce a while loop here
    node3.children = {&node4};
    node2.children = {&node4};
    node1.children = {&node3, &node2};
    root.children = {&node1};

    root.parents = {};
    node1.parents = {&root, &node4}; // introduce a while loop here
    node2.parents = {&node1};
    node3.parents = {&node1};
    node4.parents = {&node2, &node3};
    node5.parents = {&node4};

    writePkb.writeCFG(1, root);
    writePkb.writeCFG(2, root);
    writePkb.writeCFG(3, node1);
    writePkb.writeCFG(4, node1);
    writePkb.writeCFG(5, node2);
    writePkb.writeCFG(6, node2);
    writePkb.writeCFG(7, node3);
    writePkb.writeCFG(8, node3);
    writePkb.writeCFG(9, node3);
    writePkb.writeCFG(10, node4);
    writePkb.writeCFG(11, node4);
    writePkb.writeCFG(12, node4);
    writePkb.writeCFG(13, node5);

    SECTION("CFGStorage: Next(n1, n2) given n1 find n2") {
        std::vector<StmtNum> res1 = readPkb.getNextRHS(11);
        std::vector<StmtNum> expected1 = {12};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<StmtNum> res2 = readPkb.getNextRHS(12);
        std::vector<StmtNum> expected2 = {3, 13};
        REQUIRE(unit_testing_utils::equals(expected2, res2));
    }

    SECTION("CFGStorage: Next(n1, n2) given n2 find n1") {
        std::vector<StmtNum> res1 = readPkb.getNextLHS(3);
        std::vector<StmtNum> expected1 = {2, 12};
        REQUIRE(unit_testing_utils::equals(expected1, res1));
    }

    SECTION("CFGStorage: Next*(n1, n2) given n1 find n2") {
        std::vector<StmtNum> res1 = readPkb.getNextTRHS(11);
        std::vector<StmtNum> expected1 = {12, 13, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        REQUIRE(unit_testing_utils::equals(expected1, res1));
    }

    SECTION("CFGStorage: Next*(n1, n2) given n2 find n1") {
        std::vector<StmtNum> res2 = readPkb.getNextTLHS(4);
        std::vector<StmtNum> expected2 = {3, 2, 1, 12, 11, 10, 9, 8, 7, 6, 5, 4};
        REQUIRE(unit_testing_utils::equals(expected2, res2));
    }
}
