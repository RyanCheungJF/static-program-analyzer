#include "FollowsStorage.h"

void FollowsStorage::writeFollows(StmtNum followee, StmtNum follower) {

    // defensive check to add the relationship if it does not exist yet.
    // but if we can guarantee that SP gives us non-duplicates, we can omit this part to improve runtime efficiency
    /*
    if (followee_follower.find(followee) == followee_follower.end()) {
        followee_follower.insert({ followee, follower});
    }
    if (follower_followee.find(follower) == follower_followee.end()) {
        follower_followee.insert({ follower, followee});
    }
     */

    // assume that SP gives us non-duplicates
    followee_follower.insert({ followee, follower});
    follower_followee.insert({ follower, followee});
    return;
}

bool FollowsStorage::checkFollows(StmtNum followee, StmtNum follower) {

    // followee does not exist in table
    if (followee_follower.find(followee) == followee_follower.end()) {
        return false;
    }

    // follower does not exist in table
    if (follower_followee.find(follower) == follower_followee.end()) {
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
