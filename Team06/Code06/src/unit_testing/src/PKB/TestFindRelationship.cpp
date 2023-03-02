#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;

TEST_CASE("findRelationship(shared_ptr<Relationship> rs), Follows and FollowsT") {
  WritePKB writePkb;
  ReadPKB readPkb;
  PKB pkb;
  pkb.initializePkb();
  writePkb.setInstancePKB(pkb);
  readPkb.setInstancePKB(pkb);

  // 3 follows 2, 2 follows 1
  // Follows(1, _) should return {1, 2}
  // Follows*(1, _) should return {1, 2}, {1, 3}

  // Follows
  std::vector<Parameter> params1 = {Parameter("1", AppConstants::FIXED_INT),
                                    Parameter("_", AppConstants::WILDCARD)};
  shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::FOLLOWS, params1);

  // FollowsT
  std::vector<Parameter> params2 = {Parameter("1", AppConstants::FIXED_INT),
                                    Parameter("_", AppConstants::WILDCARD)};
  shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::FOLLOWST, params2);

  writePkb.setFollows(1, 2);
  writePkb.setFollowsT(1, {2, 3});

  auto res1 = readPkb.findRelationship(rs1);
  auto res2 = readPkb.findRelationship(rs2);

}