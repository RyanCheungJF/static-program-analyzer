#include "FollowsAPI.h"

void FollowsAPI::setFollows(StmtNum left, StmtNum right) {
    store.writeFollows(left, right);
    return;
}

bool FollowsAPI::checkFollows(StmtNum left, StmtNum right) {
    return store.readFollows(left, right);
}