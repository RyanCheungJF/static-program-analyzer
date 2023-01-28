#include "Follows.h"

void Follows::setFollows(StmtNum followee, StmtNum follower) {
    store.writeFollows(followee, follower);
    return;
}

bool Follows::checkFollows(StmtNum followee, StmtNum follower) {
    return store.checkFollows(followee, follower);
}

StmtNum Follows::getFollower(StmtNum followee) {
    return store.getFollower(followee);
}

StmtNum Follows::getFollowee(StmtNum follower) {
    return store.getFollowee(follower);
}