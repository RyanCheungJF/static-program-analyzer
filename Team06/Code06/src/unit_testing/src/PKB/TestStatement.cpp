#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("Check that all statements are recorded") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);


    REQUIRE(false);
}

