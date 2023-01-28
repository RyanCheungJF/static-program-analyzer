#include "FollowsStorage.h"

void FollowsStorage::writeFollows(StmtNum followee, StmtNum follower) {
    // does not exist yet
    if (follower_followee.find(followee) == follower_followee.end()) {
        follower_followee.insert({ followee, follower});
    }
    if (followee_follower.find(follower) == followee_follower.end()) {
        followee_follower.insert({ follower, followee});
    }
    return;
}

bool FollowsStorage::checkFollows(StmtNum followee, StmtNum follower) {

    // does not exist yet
    if (follower_followee.find(followee) == follower_followee.end()) {
        return false;
    }
    return follower_followee.at(followee) == follower;
}

StmtNum FollowsStorage::getFollower(StmtNum followee) {

    // does not exist yet
    if (follower_followee.find(followee) == follower_followee.end()) {
        return (StmtNum) -1;
    }
    return follower_followee.at(followee);
}

StmtNum FollowsStorage::getFollowee(StmtNum follower) {

    // does not exist yet
    if (followee_follower.find(follower) == followee_follower.end()) {
        return -1;
    }
    return followee_follower.at(follower);
}
