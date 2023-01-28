#include "ReadPKB.h"

void ReadPKB::setInstancePKB(PKB &pkb) {
    if (this -> pkbInstance != NULL) {
        return;
    }
    this -> pkbInstance = &pkb;
    return;
}

bool ReadPKB::checkFollows(StmtNum left, StmtNum right) {
    return pkbInstance -> followsApi.checkFollows(left, right);
}

StmtNum ReadPKB::getFollower(StmtNum followee) {
    return pkbInstance -> followsApi.getFollower(followee);
}

StmtNum ReadPKB::getFollowee(StmtNum follower) {
    return pkbInstance -> followsApi.getFollowee(follower);
}

bool ReadPKB::getFollowsT(StmtNum left, StmtNum right) {
    return true;
}