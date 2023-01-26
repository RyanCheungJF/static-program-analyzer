

#include "catch.hpp"
#include "../../spa/src/PKB/WritePKBAPI.h"
#include "../../spa/src/PKB/ReadPKB.h"

using namespace std;

void require(bool b) {
    REQUIRE(b);
}

TEST_CASE("Write Follows(1, 2), Read (1, 2)") {
    WritePKBAPI writeApi;
    ReadPKB readApi;

    writeApi.setFollows(1, 2);
    bool res = readApi.getFollows(1, 2);

    require(res == true);
}

TEST_CASE("Write Follows(1, 2), Read (1, 3)") {
    WritePKBAPI writeApi;
    ReadPKB readApi;

    writeApi.setFollows(1, 2);
    bool res = readApi.getFollows(1, 3);

    require(res == false);
//    require(res == true); // purposely wrong
}


