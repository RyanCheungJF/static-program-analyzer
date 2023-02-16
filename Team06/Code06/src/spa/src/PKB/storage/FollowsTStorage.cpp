#include "FollowsTStorage.h"

void FollowsTStorage::write(StmtNum followee, std::unordered_set<StmtNum> followers) {
    leftToRightMap[followee].insert(followers.begin(), followers.end());
    for (StmtNum follower: followers) {
        rightToLeftMap[follower].insert(followee);
    }
}

