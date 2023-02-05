#include "catch.hpp"
#include <string>
#include "PKB/PKB.h"
#include "PKB/ReadPKB.h"
#include "PKB/WritePKB.h"
#include "qps/QPS.h"
using namespace std;

TEST_CASE("adhoc") {
	QPS qps;
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    FollowsStorage fs;
    pkb.followsStorage = &fs;
    StmtStorage sts;
    pkb.statementStorage = &sts;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setFollows(1, 2);

    string input = "stmt s; Select s such that Follows(1, s)";
    vector<string> expected{ "2" };
    vector<string> res = qps.processQueries(input, readPkb);
    for (string r : res) {
        cout << r << "\n";
    }
    REQUIRE(true);
}