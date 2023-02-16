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
    leftToRightMap.insert({ followee, {follower} });
    rightToLeftMap.insert({ follower, {followee} });
}
