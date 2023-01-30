#include "FollowsT.h"

void FollowsT::writeFollowsT(std::vector<std::pair<StmtNum, StmtNum>> followee_follower) {
    store.writeFollowsT(followee_follower);
    return;
}

bool FollowsT::checkFollowsT(StmtNum followee, StmtNum follower) {
    return store.checkFollowsT(followee, follower);
}

std::unordered_set<StmtNum> FollowsT::getFollowersT(StmtNum followee) {
    return store.getFollowers(followee);
}

std::unordered_set<StmtNum> FollowsT::getFolloweesT(StmtNum follower) {
    return store.getFollowees(follower);
}