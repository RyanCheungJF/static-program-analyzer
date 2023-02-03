#include "FollowsStorage.h"

void FollowsStorage::write(StmtNum followee, StmtNum follower) {

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

bool FollowsStorage::exists(StmtNum followee, StmtNum follower) {

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

std::vector<StmtNum> FollowsStorage::getRightWildcard(StmtNum followee) {

    // followee does not exist in table
    if (followee_follower.find(followee) == followee_follower.end()) {
        return std::vector<StmtNum>();
    }
    return std::vector<StmtNum> {followee_follower.at(followee)};
}

std::vector<StmtNum> FollowsStorage::getLeftWildcard(StmtNum follower) {

    // follower does not exist in table
    if (follower_followee.find(follower) == follower_followee.end()) {
        return std::vector<StmtNum>();
    }
    return std::vector<StmtNum> {follower_followee.at(follower)};
}
