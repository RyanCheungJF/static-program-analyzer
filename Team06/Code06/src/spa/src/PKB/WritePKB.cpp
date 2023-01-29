#include "WritePKB.h"

void WritePKB::setInstancePKB(PKB &pkb) {
    if (this -> pkbInstance != NULL) {
        return;
    }
    this -> pkbInstance = &pkb;
    return;
}

void WritePKB::setFollows(StmtNum followee, StmtNum follower) {
    pkbInstance -> followsApi.setFollows(followee, follower);
}

void WritePKB::setFollowsT(StmtNum follower, StmtNum followee) {

}

void WritePKB::setParent(StmtNum parent, StmtNum children) {
    pkbInstance->parentApi.setParent(parent, children);
}
