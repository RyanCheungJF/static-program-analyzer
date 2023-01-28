

#include "catch.hpp"
#include "../../spa/src/PKB/WritePKB.h"
#include "../../spa/src/PKB/ReadPKB.h"
#include "../../spa/src/PKB/PKB.h"

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
    bool res = readPkb.getFollows(1, 2);
    REQUIRE(res);
}

TEST_CASE("Write Follows(1, 2), Read (1, 3)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);
    bool res = readPkb.getFollows(1, 3);
    REQUIRE(res == false);
}


