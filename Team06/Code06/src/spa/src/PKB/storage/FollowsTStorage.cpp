#include "FollowsTStorage.h"

void FollowsTStorage::writeFollowsT(StmtNum followee, StmtNum follower) {
    followee_followers[followee].insert(follower);
    follower_followees[follower].insert(followee);
    return;
}

void FollowsTStorage::writeFollowsT(StmtNum followee, std::vector<StmtNum> followers) {
    for (StmtNum follower: followers) {
        followee_followers[followee].insert(follower);
        follower_followees[follower].insert(followee);
    }
    return;
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