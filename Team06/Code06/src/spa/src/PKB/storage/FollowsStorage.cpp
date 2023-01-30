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

    // follower does not exist in table
    if (follower_followee.find(follower) == follower_followee.end()) {
        return false;
    }

    // followee does not exist in table
    if (followee_follower.find(followee) == followee_follower.end()) {
        return false;
    }

    return follower_followee.at(follower) == followee && followee_follower.at(followee) == follower;
}

StmtNum FollowsStorage::getFollower(StmtNum followee) {

    // followee does not exist in table
    if (followee_follower.find(followee) == followee_follower.end()) {
        return (StmtNum) -1;
    }
    return followee_follower.at(followee);
}

StmtNum FollowsStorage::getFollowee(StmtNum follower) {

    // follower does not exist in table
    if (follower_followee.find(follower) == follower_followee.end()) {
        return -1;
    }
    return follower_followee.at(follower);
}
