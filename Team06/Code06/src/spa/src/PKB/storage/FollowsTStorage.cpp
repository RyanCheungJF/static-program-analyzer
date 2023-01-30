#include "FollowsTStorage.h"

void FollowsTStorage::writeFollowsT(std::vector<std::pair<StmtNum, StmtNum>> followee_follower) {
    for (std::pair<StmtNum, StmtNum> p: followee_follower) {
        StmtNum followee = p.first;
        StmtNum follower = p.second;
        followee_followers[followee].insert(follower);
        follower_followees[follower].insert(followee);
    }
    return;
}

bool FollowsTStorage::checkFollowsT(StmtNum followee, StmtNum follower) {
    return false;
}

std::unordered_set<StmtNum> FollowsTStorage::getFollowers(StmtNum followee) {
    if (followee_followers.find(followee) == followee_followers.end()) {
        std::unordered_set<StmtNum> emptySet;
        return emptySet;
    }
    return followee_followers[followee];
}

std::unordered_set<StmtNum> FollowsTStorage::getFollowees(StmtNum follower) {
    if (follower_followees.find(follower) == follower_followees.end()) {
        std::unordered_set<StmtNum> emptySet;
        return emptySet;
    }
    return follower_followees[follower];

}