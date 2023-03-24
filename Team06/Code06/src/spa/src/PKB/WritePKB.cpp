#include "WritePKB.h"

void WritePKB::setInstancePKB(PKB& pkb) {
    if (this->pkbInstance != NULL) {
    }
    this->pkbInstance = &pkb;
}

void WritePKB::setFollows(StmtNum followee, StmtNum follower) {
    pkbInstance->setFollows(followee, follower);
}

void WritePKB::setFollowsT(StmtNum followee, std::unordered_set<StmtNum>& followers) {
    pkbInstance->setFollowsT(followee, followers);
}

void WritePKB::setParent(StmtNum parent, StmtNum children) {
    pkbInstance->setParent(parent, children);
}

void WritePKB::setParentT(StmtNum parent, std::unordered_set<StmtNum>& children) {
    pkbInstance->setParentT(parent, children);
}

void WritePKB::setProcedure(ProcName p, std::unordered_set<StmtNum>& lines) {
    pkbInstance->setProcedure(p, lines);
}

void WritePKB::setStatement(Stmt s, StmtNum line) {
    pkbInstance->setStatement(s, line);
}

void WritePKB::setEntity(StmtNum num, std::unordered_set<Ent>& entities) {
    pkbInstance->setEntity(num, entities);
}

void WritePKB::setConstant(StmtNum num, std::unordered_set<Const>& constants) {
    pkbInstance->setConstant(num, constants);
}

void WritePKB::setCall(StmtNum callLine, ProcName procedure_being_called) {
    pkbInstance->setCall(callLine, procedure_being_called);
}

void WritePKB::setCalls(ProcName caller, std::unordered_set<ProcName>& callees) {
    pkbInstance->setCalls(caller, callees);
}

void WritePKB::setCallsT(ProcName caller, std::unordered_set<ProcName>& callees) {
    pkbInstance->setCallsT(caller, callees);
}

void WritePKB::setUsesS(StmtNum num, std::unordered_set<Ent>& entities) {
    pkbInstance->setUsesS(num, entities);
}

void WritePKB::setUsesP(ProcName name, std::unordered_set<Ent>& entities) {
    pkbInstance->setUsesP(name, entities);
}

void WritePKB::setModifiesS(StmtNum num, std::unordered_set<Ent>& entities) {
    pkbInstance->setModifiesS(num, entities);
}

void WritePKB::setModifiesP(ProcName name, std::unordered_set<Ent>& entities) {
    pkbInstance->setModifiesP(name, entities);
}

void WritePKB::setIfPattern(StmtNum num, std::unordered_set<Ent>& variables) {
    pkbInstance->writeIfPattern(num, variables);
}

void WritePKB::setWhilePattern(StmtNum num, std::unordered_set<Ent>& variables) {
    pkbInstance->writeWhilePattern(num, variables);
}

void WritePKB::writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer) {
    pkbInstance->writePattern(lhs, num, std::move(pointer));
}

void WritePKB::writeCFG(
    ProcName name, std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& graph) {
    pkbInstance->writeCFG(name, graph);
}
