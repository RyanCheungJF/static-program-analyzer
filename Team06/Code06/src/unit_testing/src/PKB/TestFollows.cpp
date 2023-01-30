

#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/PKB.h"

using namespace std;

//void require(bool b) {
//    REQUIRE(b);
//}

TEST_CASE("Write Follows(1, 2), Read (1, 2)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    bool res = readPkb.checkFollows(1, 2);
    REQUIRE(res);
}

TEST_CASE("Write Follows(1, 2), Read (1, 3)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    bool res = readPkb.checkFollows(1, 3);
    REQUIRE(res == false);
}

TEST_CASE("Write Follows(1, 2), Check that follower of 1 is 2") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    StmtNum res = readPkb.getFollower(1);
    REQUIRE(res == 2);
}

TEST_CASE("Write Follows(1, 2), Check that followee of 2 is 1") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    StmtNum res = readPkb.getFollowee(2);
    REQUIRE(res == 1);
}

TEST_CASE("Write Follows(1, 2), Check that follower of 3 returns a -1 to indicate error") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    StmtNum res = readPkb.getFollower(3);
    REQUIRE(res == -1);
}

TEST_CASE("Write Follows(1, 2), Check that followee of 3 returns a -1 to indicate error") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    StmtNum res = readPkb.getFollowee(3);
    REQUIRE(res == -1);
}

TEST_CASE("Ensure that PKB pointer in WritePKB is set to first pkb instance and not overwritten") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb1;
    writePkb.setInstancePKB(pkb1);
    readPkb.setInstancePKB(pkb1);

    writePkb.setFollows(1, 2);

    PKB pkb2;
    writePkb.setInstancePKB(pkb2);

    bool res = readPkb.checkFollows(1, 2);
    REQUIRE(res);
}

TEST_CASE("Ensure that PKB pointer in ReadPKB is set to first pkb instance and not overwritten") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb1;
    writePkb.setInstancePKB(pkb1);
    readPkb.setInstancePKB(pkb1);

    writePkb.setFollows(1, 2);

    PKB pkb2;
    readPkb.setInstancePKB(pkb2);

    bool res = readPkb.checkFollows(1, 2);
    REQUIRE(res);
}


//TODO
/**
 * Given a PQL query of `if i; Select i such that Follows*(2, i)`
 * We want to find all if-statements that follows* line 2
 */
TEST_CASE("Check that we can find an if statement from a Follows relationship") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    REQUIRE(true);
}
