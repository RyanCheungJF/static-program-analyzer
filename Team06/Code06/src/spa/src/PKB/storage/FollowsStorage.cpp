#include "FollowsStorage.h"

void FollowsStorage::write(StmtNum followee, StmtNum follower) {
    leftToRightMap.insert({ followee, {follower} });
    rightToLeftMap.insert({ follower, {followee} });
}
