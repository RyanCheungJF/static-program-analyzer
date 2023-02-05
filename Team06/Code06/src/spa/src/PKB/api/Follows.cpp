#include "Follows.h"

void Follows::setFollows(StmtNum followee, StmtNum follower) {
    store.write(followee, follower);
    return;
}

bool Follows::checkFollows(StmtNum followee, StmtNum follower) {
    return store.exists(followee, follower);
}

std::vector<StmtNum> Follows::getFollower(StmtNum followee) {
    return store.getRightWildcard(followee);
}

std::vector<StmtNum> Follows::getFollowee(StmtNum follower) {
    return store.getLeftWildcard(follower);
}