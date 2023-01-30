#include "WritePKB.h"

void WritePKB::setInstancePKB(PKB &pkb) {
    if (this->pkbInstance != NULL) {
        return;
    }
    this->pkbInstance = &pkb;
    return;
}

void WritePKB::setFollows(StmtNum followee, StmtNum follower) {
    pkbInstance->followsApi.setFollows(followee, follower);
    return;
}

void WritePKB::setFollowsT(std::vector<std::pair<StmtNum, StmtNum>> followee_follower) {
    pkbInstance->followsTApi.writeFollowsT(followee_follower);
    return;
}

void WritePKB::setParent(StmtNum parent, StmtNum children) {
    pkbInstance->parentApi.setParent(parent, children);
}

void WritePKB::setStatement(Stmt s, std::vector<StmtNum> lines) {
    pkbInstance->statementApi.writeStatement(s, lines);
    return;
}
