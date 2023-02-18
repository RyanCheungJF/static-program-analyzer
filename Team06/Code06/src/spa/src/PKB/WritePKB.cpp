#include "WritePKB.h"

void WritePKB::setInstancePKB(PKB& pkb) {
    if (this->pkbInstance != NULL) {

    }
    this->pkbInstance = &pkb;
}

void WritePKB::setFollows(StmtNum followee, StmtNum follower) {
    pkbInstance->followsStorage->write(followee, follower);
}

void WritePKB::setFollowsT(StmtNum followee, std::unordered_set<StmtNum> followers) {
    pkbInstance->followsTStorage->write(followee, followers);
}

void WritePKB::setParent(StmtNum parent, StmtNum children) {
    pkbInstance->parentStorage->write(parent, children);
}

void WritePKB::setParentT(StmtNum parent, std::unordered_set<StmtNum> children) {
    pkbInstance->parentTStorage->write(parent, children);
}

void WritePKB::setStatement(Stmt s, std::unordered_set<StmtNum> lines) {
    pkbInstance->statementStorage->writeStatement(s, lines);
}

void WritePKB::setProcedure(ProcName p, std::unordered_set<StmtNum> lines) {
    pkbInstance->procedureStorage->writeProcedure(p, lines);
}

void WritePKB::setStatement(Stmt s, StmtNum line) {
    pkbInstance->statementStorage->writeStatement(s, line);
}

void WritePKB::setEntity(StmtNum num, std::unordered_set<Ent> entities) {
    pkbInstance->entityStorage->writeEntity(num, entities);
}

void WritePKB::setConstant(StmtNum num, std::unordered_set<Const> constants) {
    pkbInstance->constantStorage->writeConstant(num, constants);
}

void WritePKB::setCall(StmtNum callLine, ProcName procedure_being_called) {
    pkbInstance->callStorage->writeCall(callLine, procedure_being_called);
}

void WritePKB::setUsesS(StmtNum num, std::unordered_set<Ent> entities) {
    pkbInstance->usesStorage->writeS(num, entities);
}

void WritePKB::setUsesP(ProcName name, std::unordered_set<Ent> entities) {
    pkbInstance->usesStorage->writeP(name, entities);
}

void WritePKB::setModifiesS(StmtNum num, std::unordered_set<Ent> entities) {
    pkbInstance->modifiesStorage->writeS(num, entities);
}

void WritePKB::setModifiesP(ProcName name, std::unordered_set<Ent> entities) {
    pkbInstance->modifiesStorage->writeP(name, entities);
}

void WritePKB::writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer) {
    pkbInstance->patternStorage->writePattern(lhs, num, std::move(pointer));
}
