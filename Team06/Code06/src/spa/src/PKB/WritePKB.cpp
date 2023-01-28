#include "WritePKB.h"

void WritePKB::setInstancePKB(PKB &pkb) {
    if (this -> pkbInstance != NULL) {
        return;
    }
    this -> pkbInstance = &pkb;
    return;
}

void WritePKB::setFollows(StmtNum left, StmtNum right) {
    pkbInstance -> followsApi.setFollows(left, right);
}

void WritePKB::setFollowsT(StmtNum left, StmtNum right) {

}
