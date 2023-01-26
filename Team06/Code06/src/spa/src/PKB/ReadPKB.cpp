#include "ReadPKB.h"

bool ReadPKB::getFollows(StmtNum left, StmtNum right) {
    return pkbInstance.followsApi.checkFollows(left, right);
}

bool ReadPKB::getFollowsT(StmtNum left, StmtNum right) {
    return true;
}