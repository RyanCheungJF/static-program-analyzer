#include "WritePKB.h"

void WritePKB::setInstancePKB(PKB &pkb) {
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

void WritePKB::setProcedure(Proc p, std::unordered_set<StmtNum> lines) {
    pkbInstance->procedureStorage->writeProcedure(p, lines);
}

void WritePKB::setStatement(Stmt s, StmtNum line) {
    pkbInstance->statementStorage->writeStatement(s, line);
}

void WritePKB::setEntity(StmtNum num, std::unordered_set<Ent> entities) {
    pkbInstance->entityStorage->writeEntity(num, entities);
}

//void WritePKB::setEntity(Ent e, StmtNum line) {
//    pkbInstance->entityStorage->writeEntity(e, line);
//}

//void WritePKB::setProcedure(Proc p, StmtNum num) {
//    pkbInstance->procedureStorage->writeProcedure(p, num);
//}

void WritePKB::setConstant(StmtNum num, std::unordered_set<Const> constants) {
    pkbInstance->constantStorage->writeConstant(num, constants);
}

void WritePKB::setCall(StmtNum callLine, Proc procedure_being_called) {
    pkbInstance->callStorage->writeCall(callLine, procedure_being_called);
}

void WritePKB::setUsesS(StmtNum num, std::unordered_set<Ent> entities) {
    pkbInstance->usesStorage->writeUsesS(num, entities);
}

void WritePKB::setUsesP(ProcName name, std::unordered_set<Ent> entities) {
    pkbInstance->usesStorage->writeUsesP(name, entities);
}

void WritePKB::setModifiesS(StmtNum num, std::unordered_set<Ent> entities) {
    pkbInstance->modifiesStorage->writeModifiesS(num, entities);
}

void WritePKB::setModifiesP(ProcName name, std::unordered_set<Ent> entities) {
    pkbInstance->modifiesStorage->writeModifiesP(name, entities);
}

void WritePKB::writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer) {
    pkbInstance->patternStorage->writePattern(lhs, num, std::move(pointer));
}

std::unique_ptr<Expression> WritePKB::buildSubtree(std::string rhs) {
    return std::move(pkbInstance->patternStorage->buildSubtree(rhs));
}

