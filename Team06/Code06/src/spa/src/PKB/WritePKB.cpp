#include "WritePKB.h"

void WritePKB::setInstancePKB(PKB &pkb) {
    if (this->pkbInstance != NULL) {
        return;
    }
    this->pkbInstance = &pkb;
    return;
}

void WritePKB::setFollows(StmtNum followee, StmtNum follower) {
    pkbInstance->followsStorage->write(followee, follower);
    return;
}

void WritePKB::setFollowsT(StmtNum followee, std::vector<StmtNum> followers) {
    pkbInstance->followsTStorage->write(followee, followers);
}

void WritePKB::setParent(StmtNum parent, StmtNum children) {
    pkbInstance->parentStorage->write(parent, children);
}

void WritePKB::setParentT(StmtNum parent, std::vector<StmtNum> children) {
    pkbInstance->parentTStorage->write(parent, children);
}

void WritePKB::setStatement(Stmt s, std::vector<StmtNum> lines) {
    pkbInstance->statementStorage->writeStatement(s, lines);
    return;
}

void WritePKB::setProcedure(Proc p, std::vector<StmtNum> lines) {
    pkbInstance->procedureStorage->writeProcedure(p, lines);
    return;
}

void WritePKB::setStatement(Stmt s, StmtNum line) {
    pkbInstance->statementStorage->writeStatement(s, line);
    return;
}

void WritePKB::setEntity(StmtNum line, std::vector<Ent> entities) {
    for (Ent e: entities) {
        setEntity(e, line);
    }
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

void WritePKB::setConstant(Const c, StmtNum line) {
    pkbInstance->constantStorage->writeConstant(c, line);
    return;
}

void WritePKB::setConstant(StmtNum line, std::vector<Const> constants) {
    for (Const c: constants) {
        pkbInstance->constantStorage->writeConstant(c, line);
    }
    return;
}

void WritePKB::setCall(StmtNum callLine, Proc procedure_being_called) {
    pkbInstance->callStorage->writeCall(callLine, procedure_being_called);
}

void WritePKB::setUses(StmtNum num, std::vector<Ent> entities) {
    pkbInstance->usesStorage->write(num, entities);
}

void WritePKB::writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer) {
    pkbInstance->patternStorage->writePattern(lhs, num, std::move(pointer));
    return;
}

std::unique_ptr<Expression> WritePKB::buildSubtree(std::string rhs) {
    return std::move(pkbInstance->patternStorage->buildSubtree(rhs));
}

