#include "ReadPKB.h"

void ReadPKB::setInstancePKB(PKB &pkb) {
    if (this -> pkbInstance != NULL) {
        return;
    }
    this -> pkbInstance = &pkb;
    return;
}

bool ReadPKB::getFollows(StmtNum left, StmtNum right) {
    return pkbInstance -> followsApi.checkFollows(left, right);
}

bool ReadPKB::getFollowsT(StmtNum left, StmtNum right) {
    return true;
}