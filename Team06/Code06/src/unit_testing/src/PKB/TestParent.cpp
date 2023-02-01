#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("Write Parent(1, 2), Read Parent(1, 2)") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Parent par;
    pkb.parentApi = &par;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    bool res = readPkb.checkParent(1, 2);
    REQUIRE(res);
}

TEST_CASE("Write Parent(1, 2), Read Parent(1, 3)") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Parent par;
    pkb.parentApi = &par;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    bool res = readPkb.checkParent(1, 3);
    REQUIRE(!res);
}

TEST_CASE("Write Parent(1, 2), Read Parent(3, 2)") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Parent par;
    pkb.parentApi = &par;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    bool res = readPkb.checkParent(3, 2);
    REQUIRE(!res);
}

TEST_CASE("Write Parent(1, 2), Write Parent(1, 3), Get Children of 1") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Parent par;
    pkb.parentApi = &par;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    writePkb.setParent(1, 3);
    std::vector<StmtNum> res = readPkb.getChildren(1);
    std::vector<StmtNum> check{ 2, 3 };
    REQUIRE(res == check);
}

TEST_CASE("Write Parent(1, 2), Write Parent(1, 3), Get Children of 2") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Parent par;
    pkb.parentApi = &par;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    writePkb.setParent(1, 3);
    std::vector<StmtNum> res = readPkb.getChildren(2);
    std::vector<StmtNum> check;
    REQUIRE(res == check);
}

TEST_CASE("Write Parent(1, 2), Get children of non-existant statement number") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Parent par;
    pkb.parentApi = &par;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    std::vector<StmtNum> res = readPkb.getChildren(3);
    std::vector<StmtNum> check;
    REQUIRE(res == check);
}

TEST_CASE("Write Parent(1, 2), Get Parent of 2") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Parent par;
    pkb.parentApi = &par;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    StmtNum res = readPkb.getParent(2);
    REQUIRE(res == 1);
}

TEST_CASE("Write Parent(1, 2), Get Parent of non-existant statement number") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Parent par;
    pkb.parentApi = &par;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    StmtNum res = readPkb.getParent(3);
    REQUIRE(res == -1);
}

TEST_CASE("Write Parent(1, 2), Get Parent of 1") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Parent par;
    pkb.parentApi = &par;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setParent(1, 2);
    StmtNum res = readPkb.getParent(1);
    REQUIRE(res == -1);
}

