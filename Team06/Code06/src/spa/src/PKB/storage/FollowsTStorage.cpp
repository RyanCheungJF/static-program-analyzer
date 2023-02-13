#include "FollowsTStorage.h"


void FollowsTStorage::write(StmtNum followee, std::unordered_set<StmtNum> followers) {
    followeeFollowersMap[followee].insert(followers.begin(), followers.end());
    for (StmtNum n: followers) {
        followerFolloweesMap[n].insert(followee);
    }
}

bool FollowsTStorage::exists(StmtNum followee, StmtNum follower) {

    // meaning the followee does not exist
    if (followeeFollowersMap.find(followee) == followeeFollowersMap.end()) {
        return false;
    }
    return followeeFollowersMap[followee].find(follower) != followeeFollowersMap[followee].end();
}

std::unordered_set<StmtNum> FollowsTStorage::getRightWildcard(StmtNum followee) {
    // followee does not exist
    if (followeeFollowersMap.find(followee) == followeeFollowersMap.end()) {
        std::unordered_set<StmtNum> emptySet;
        return emptySet;
    }
    return followeeFollowersMap[followee];
    

}

std::unordered_set<StmtNum> FollowsTStorage::getLeftWildcard(StmtNum follower) {
    // follower does not exist
    if (followerFolloweesMap.find(follower) == followerFolloweesMap.end()) {
        std::unordered_set<StmtNum> emptySet;
        return emptySet;
    }
    return followerFolloweesMap[follower];
}

std::pair<std::vector<StmtNum>, std::vector<StmtNum>> FollowsTStorage::getAllPairs() {
    std::vector<StmtNum> followees;
    std::vector<StmtNum> followers;
    for (auto followeeFollowers : followeeFollowersMap) {
        StmtNum followee = followeeFollowers.first;
        for (auto follower : followeeFollowers.second) {
            followees.push_back(followee);
            followers.push_back(follower);
        }
        
    }
    return { followees, followers };
}

