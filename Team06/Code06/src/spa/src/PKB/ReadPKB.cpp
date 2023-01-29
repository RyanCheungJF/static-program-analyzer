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

bool ReadPKB::checkParent(StmtNum parent, StmtNum child) {
    return pkbInstance->parentApi.checkParent(parent, child);
}

std::vector<StmtNum> ReadPKB::getChildren(StmtNum parent) {
    return pkbInstance->parentApi.getChildren(parent);
}

StmtNum ReadPKB::getParent(StmtNum child) {
    return pkbInstance->parentApi.getParent(child);
}