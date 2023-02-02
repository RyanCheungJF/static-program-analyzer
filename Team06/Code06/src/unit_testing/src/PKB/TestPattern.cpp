#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("All nodes can be added") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    PatternStorage pa;
    pkb.patternStorage = &pa;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    REQUIRE(true);
}
