#include "FollowsTStorage.h"

void FollowsTStorage::writeFollowsT(StmtNum followee, StmtNum follower) {}

void FollowsTStorage::writeFollowsT(StmtNum followee, std::vector<StmtNum> followers) {}

std::unordered_set<StmtNum> FollowsTStorage::getFollowers(StmtNum followee) {}

std::unordered_set<StmtNum> FollowsTStorage::getFollowees(StmtNum follower) {}