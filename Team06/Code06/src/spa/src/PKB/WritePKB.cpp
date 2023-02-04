#include "WritePKB.h"

void WritePKB::setInstancePKB(PKB &pkb) {
    if (this->pkbInstance != NULL) {
        return;
    }
    this->pkbInstance = &pkb;
    return;
}

void WritePKB::setFollows(StmtNum followee, StmtNum follower) {
    pkbInstance->followsApi->setFollows(followee, follower);
    return;
}

void WritePKB::setFollowsT(std::vector<std::pair<StmtNum, StmtNum>> followee_follower) {
    pkbInstance->followsTApi->writeFollowsT(followee_follower);
    return;
}

void WritePKB::setParent(StmtNum parent, StmtNum children) {
    pkbInstance->parentApi->setParent(parent, children);
}

void WritePKB::setStatement(Stmt s, std::vector<StmtNum> lines) {
    pkbInstance->statementApi->writeStatement(s, lines);
    return;
}

void WritePKB::setEntity(Ent e, std::vector<StmtNum> lines) {
    pkbInstance->entityStorage->writeEntity(e, lines);
    return;
}

void WritePKB::setProcedure(Proc p, std::vector<StmtNum> lines) {
    pkbInstance->procedureStorage->writeProcedure(p, lines);
    return;
}

void WritePKB::setStatement(Stmt s, StmtNum line) {
    pkbInstance->statementApi->writeStatement(s, line);
    return;
}

void WritePKB::setEntity(Ent e, StmtNum line) {
    pkbInstance->entityStorage->writeEntity(e, line);
    return;
}

void WritePKB::setProcedure(Proc p, StmtNum line) {
    pkbInstance->procedureStorage->writeProcedure(p, line);
    return;
}
