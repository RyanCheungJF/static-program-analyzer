#include "catch.hpp"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

using namespace std;

TEST_CASE("Check that all followers are recorded in the followee") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::vector<std::pair<StmtNum, StmtNum>> followee_follower;
    followee_follower.push_back({1, 2});
    followee_follower.push_back({1, 3});
    followee_follower.push_back({1, 4});

    writePkb.setFollowsT(followee_follower);

    bool res = true;
    res = res && readPkb.checkFollowsT(1, 2);
    res = res && readPkb.checkFollowsT(1, 3);
    res = res && readPkb.checkFollowsT(1, 4);
    REQUIRE(res);
}
