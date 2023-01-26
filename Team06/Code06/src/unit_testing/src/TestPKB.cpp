

#include "catch.hpp"
//#include "../../spa/src/PKB/WritePKBAPI.h"
//#include "../../spa/src/PKB/ReadPKB.h"
#include "../../spa/src/PKB/PKB.h"

using namespace std;


//void require(bool b) {
//    REQUIRE(b);
//}

TEST_CASE("Write Follows(1, 2), Read (1, 2)") {

    pkbInstance.followsApi.setFollows(1, 2);
    bool res = pkbInstance.followsApi.checkFollows(1, 2);
    REQUIRE(res);
}

TEST_CASE("Write Follows(1, 2), Read (1, 3)") {

    pkbInstance.followsApi.setFollows(1, 2);
    bool res = pkbInstance.followsApi.checkFollows(1, 3);
    REQUIRE(res == false);
}


