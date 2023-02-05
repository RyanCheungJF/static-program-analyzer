#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;


TEST_CASE("Checks that write and read works for Follows") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Follows f;
    pkb.followsApi = &f;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    bool res = readPkb.checkFollows(1, 2);
    REQUIRE(res);
}

TEST_CASE("Checks that a non-existent follows relationship is detected") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Follows f;
    pkb.followsApi = &f;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    bool res = readPkb.checkFollows(1, 3);
    REQUIRE(res == false);
}

TEST_CASE("Checks that follower is correct given a followee") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Follows f;
    pkb.followsApi = &f;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    StmtNum res = readPkb.getFollower(1);
    REQUIRE(res == 2);
}

TEST_CASE("Checks that followee is correct given a follower") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Follows f;
    pkb.followsApi = &f;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    StmtNum res = readPkb.getFollowee(2);
    REQUIRE(res == 1);
}

TEST_CASE("Checks that given a followee, if it does not have a certain follower, a -1 is returned to indicate error") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Follows f;
    pkb.followsApi = &f;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    StmtNum res = readPkb.getFollower(3);
    REQUIRE(res == -1);
}

TEST_CASE("Checks that given a follower, if it does not have a certain followee, a -1 is returned to indicate error") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Follows f;
    pkb.followsApi = &f;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    StmtNum res = readPkb.getFollowee(3);
    REQUIRE(res == -1);
}

TEST_CASE("Checks that PKB pointer in WritePKB is set to first pkb instance and not overwritten") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb1;
    Follows f;
    pkb1.followsApi = &f;
    writePkb.setInstancePKB(pkb1);
    readPkb.setInstancePKB(pkb1);

    writePkb.setFollows(1, 2);

    PKB pkb2;
    Follows f2;
    pkb2.followsApi = &f2;
    writePkb.setInstancePKB(pkb2);

    bool res = readPkb.checkFollows(1, 2);
    REQUIRE(res);
}

TEST_CASE("Checks that PKB pointer in ReadPKB is set to first pkb instance and not overwritten") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb1;
    Follows f;
    pkb1.followsApi = &f;
    writePkb.setInstancePKB(pkb1);
    readPkb.setInstancePKB(pkb1);

    writePkb.setFollows(1, 2);

    PKB pkb2;
    Follows f2;
    pkb2.followsApi = &f2;
    readPkb.setInstancePKB(pkb2);

    bool res = readPkb.checkFollows(1, 2);
    REQUIRE(res);
}

TEST_CASE("Checks that different PKB running instances can point to the same API") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb1;
    Follows f;
    pkb1.followsApi = &f;
    writePkb.setInstancePKB(pkb1);

    writePkb.setFollows(1, 2);

    PKB pkb2;
    pkb2.followsApi = &f;
    readPkb.setInstancePKB(pkb2);

    bool res = readPkb.checkFollows(1, 2);
    REQUIRE(res);
}
