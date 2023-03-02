#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/utils/utils.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;
using namespace std;

TEST_CASE("Check Pattern Storage Write correctness") {
  PatternStorage ps;
  ps.writePattern(1, {"x", "y"});

  SECTION("Check Writes do not override existing values") {
    ps.writePattern(2, {"y"});

    unordered_set<StmtNum> res = ps.getStmtNums("y");

    unordered_set<StmtNum> expected = {1, 2};
    REQUIRE(res.size() == 2);
    REQUIRE(equals(res, expected));
  }
  
  SECTION("Check that writing existing values do not cause duplicates") {
    ps.writePattern(1, {"x", "y"});

    unordered_set<StmtNum> res = ps.getStmtNums("y");

    unordered_set<StmtNum> expected = {1};
    REQUIRE(res.size() == 1);
    REQUIRE(equals(res, expected));
  }
}

TEST_CASE("Check Pattern Storage Write and non-empty Gets") {
  PatternStorage ps;
  ps.writePattern(1, {"x", "y"});
  ps.writePattern(2, {"y", "z"});
  ps.writePattern(3, {"z"});

  SECTION("Get statement numbers of existing pattern") {
    unordered_set<StmtNum> res = ps.getStmtNums("y");
    unordered_set<StmtNum> expected = {1, 2};
    REQUIRE(res.size() == 2);
    REQUIRE(equals(res, expected));
  }

  SECTION("Get all statement number - variable pairs") {
    pair<vector<StmtNum>, vector<Ent>> res = ps.getAllPairs();
    vector<StmtNum> stmtNums = res.first;
    vector<Ent> vars = res.second;

    vector<StmtNum> expectedStmtNums = {1, 1, 2, 2, 3};
    REQUIRE(stmtNums.size() == 5);
    REQUIRE(equals(stmtNums, expectedStmtNums));

    vector<Ent> expectedVars = {"x", "y", "y", "z", "z"};
    REQUIRE(res.second.size() == 5);
    REQUIRE(equals(vars, expectedVars));
  }
}

TEST_CASE("Check Pattern Storage Write and empty Gets") {
  PatternStorage ps;
  ps.writePattern(1, {"x", "y"});
  ps.writePattern(2, {"y", "z"});
  ps.writePattern(3, {"z"});

  SECTION("Get statement numbers of non-existing pattern") {
    unordered_set<StmtNum> res = ps.getStmtNums("non-existing");
    REQUIRE(res.size() == 0);
  }
}

