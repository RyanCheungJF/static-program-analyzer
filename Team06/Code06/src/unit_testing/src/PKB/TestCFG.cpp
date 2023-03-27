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
        REQUIRE(readPkb.getCFG("doesNotExist")->empty());
    }

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph1 = {
        {1, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {2, 3}}}},
        {2, {{AppConstants::PARENTS, {1}}, {AppConstants::CHILDREN, {5}}}},
        {3, {{AppConstants::PARENTS, {1}}, {AppConstants::CHILDREN, {4}}}},
        {4, {{AppConstants::PARENTS, {3}}, {AppConstants::CHILDREN, {5}}}},
        {5, {{AppConstants::PARENTS, {2, 4}}, {AppConstants::CHILDREN, {}}}}};

    ProcName proc1 = "proc1";
    writePkb.writeCFG(proc1, graph1);

    auto j = *readPkb.getCFG(proc1);
    auto i = *readPkb.getCFG("proc0");

    //  SECTION("CFGStorage: Next(n1, n2) given n1 find n2") {
    //    std::vector<StmtNum> res1 = readPkb.getNextRHS(3);
    //    std::vector<StmtNum> expected1 = {4};
    //    REQUIRE(unit_testing_utils::equals(expected1, res1));
    //
    //    std::vector<StmtNum> res2 = readPkb.getNextRHS(4);
    //    std::vector<StmtNum> expected2 = {5, 7};
    //    REQUIRE(unit_testing_utils::equals(expected2, res2));
    //
    //    std::vector<StmtNum> res3 = readPkb.getNextRHS(13);
    //    std::vector<StmtNum> expected3 = {};
    //    REQUIRE(unit_testing_utils::equals(expected3, res3));
    //  }
    //
    //  SECTION("CFGStorage: Next(n1, n2) given n2 find n1") {
    //    std::vector<StmtNum> res1 = readPkb.getNextLHS(5);
    //    std::vector<StmtNum> expected1 = {4};
    //    REQUIRE(unit_testing_utils::equals(expected1, res1));
    //
    //    std::vector<StmtNum> res2 = readPkb.getNextLHS(7);
    //    std::vector<StmtNum> expected2 = {4};
    //    REQUIRE(unit_testing_utils::equals(expected2, res2));
    //
    //    std::vector<StmtNum> res3 = readPkb.getNextLHS(4);
    //    std::vector<StmtNum> expected3 = {3};
    //    REQUIRE(unit_testing_utils::equals(expected3, res3));
    //
    //    std::vector<StmtNum> res4 = readPkb.getNextLHS(10);
    //    std::vector<StmtNum> expected4 = {6, 9};
    //    REQUIRE(unit_testing_utils::equals(expected4, res4));
    //
    //    std::vector<StmtNum> res5 = readPkb.getNextLHS(1);
    //    std::vector<StmtNum> expected5 = {};
    //    REQUIRE(unit_testing_utils::equals(expected5, res5));
    //  }
}

// TEST_CASE("CFGStorage, ReadPKB, WritePKB, have while loop") {
//
//   WritePKB writePkb;
//   ReadPKB readPkb;
//   PKB pkb;
//   pkb.initializePkb();
//   writePkb.setInstancePKB(pkb);
//   readPkb.setInstancePKB(pkb);
//   SECTION("CFGStorage: Next(n1, n2) given n1 find n2") {
//     std::vector<StmtNum> res1 = readPkb.getNextRHS(11);
//     std::vector<StmtNum> expected1 = {12};
//     REQUIRE(unit_testing_utils::equals(expected1, res1));
//
//     std::vector<StmtNum> res2 = readPkb.getNextRHS(12);
//     std::vector<StmtNum> expected2 = {3, 13};
//     REQUIRE(unit_testing_utils::equals(expected2, res2));
//   }
//
//   SECTION("CFGStorage: Next(n1, n2) given n2 find n1") {
//     std::vector<StmtNum> res1 = readPkb.getNextLHS(3);
//     std::vector<StmtNum> expected1 = {2, 12};
//     REQUIRE(unit_testing_utils::equals(expected1, res1));
//   }
//
//   SECTION("CFGStorage: Next*(n1, n2) given n1 find n2") {
//     std::vector<StmtNum> res1 = readPkb.getNextTRHS(11);
//     std::vector<StmtNum> expected1 = {12, 13, 3, 4, 5, 6, 7, 8, 9, 10, 11};
//     REQUIRE(unit_testing_utils::equals(expected1, res1));
//   }
//
//   SECTION("CFGStorage: Next*(n1, n2) given n2 find n1") {
//     std::vector<StmtNum> res2 = readPkb.getNextTLHS(4);
//     std::vector<StmtNum> expected2 = {3, 2, 1, 12, 11, 10, 9, 8, 7, 6, 5, 4};
//     REQUIRE(unit_testing_utils::equals(expected2, res2));
//   }
// }
