#include "FollowsStorage.h"

void FollowsStorage::write(StmtNum followee, StmtNum follower) {

    // defensive check to add the relationship if it does not exist yet.
    // but if we can guarantee that SP gives us non-duplicates, we can omit this part to improve runtime efficiency
    /*
    if (followeeFollowerMap.find(followee) == followeeFollowerMap.end()) {
        followeeFollowerMap.insert({ followee, follower});
    }
    if (followerFolloweeMap.find(follower) == followerFolloweeMap.end()) {
        followerFolloweeMap.insert({ follower, followee});
    }
     */

    // assume that SP gives us non-duplicates
    followeeFollowerMap.insert({ followee, follower});
    followerFolloweeMap.insert({ follower, followee});
    return;
}

bool FollowsStorage::exists(StmtNum followee, StmtNum follower) {

    // followee does not exist in table
    if (followeeFollowerMap.find(followee) == followeeFollowerMap.end()) {
        return false;
    }

    // follower does not exist in table
    if (followerFolloweeMap.find(follower) == followerFolloweeMap.end()) {
        return false;
    }


    return followerFolloweeMap.at(follower) == followee && followeeFollowerMap.at(followee) == follower;
}

std::unordered_set<StmtNum> FollowsStorage::getRightWildcard(StmtNum followee) {

    // followee does not exist in table
    if (followeeFollowerMap.find(followee) == followeeFollowerMap.end()) {
        return std::unordered_set<StmtNum>();
    }
    return std::unordered_set<StmtNum> {followeeFollowerMap.at(followee)};
}

std::unordered_set<StmtNum> FollowsStorage::getLeftWildcard(StmtNum follower) {

    // follower does not exist in table
    if (followerFolloweeMap.find(follower) == followerFolloweeMap.end()) {
        return std::unordered_set<StmtNum>();
    }
    return std::unordered_set<StmtNum> {followerFolloweeMap.at(follower)};
}

std::pair<std::vector<StmtNum>, std::vector<StmtNum>> FollowsStorage::getAllPairs() {
    std::vector<StmtNum> followees;
    std::vector<StmtNum> followers;
    for (auto followeeFollower : followeeFollowerMap) {
        followees.push_back(followeeFollower.first);
        followers.push_back(followeeFollower.second);
    }
    return { followees, followers };
}
