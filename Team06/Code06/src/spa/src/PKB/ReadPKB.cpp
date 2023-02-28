#include "ReadPKB.h"

void ReadPKB::setInstancePKB(PKB &pkb) {
  if (this->pkbInstance != NULL) {
    return;
  }
  this->pkbInstance = &pkb;
}

std::vector<std::vector<std::string>>
ReadPKB::findRelationship(shared_ptr<Relationship> rs) {
  return pkbInstance->findRelationship(rs);
}

std::vector<std::string> ReadPKB::findDesignEntities(Parameter p) {
  return pkbInstance->findDesignEntities(p);
}

std::vector<std::vector<std::string>> ReadPKB::findPattern(Pattern p) {
  return pkbInstance->findPattern(p);
}

bool ReadPKB::checkStatement(Stmt stmt, StmtNum num) {
  return pkbInstance->checkStatement(stmt, num);
}

std::unordered_set<StmtNum> ReadPKB::getProcedureStatementNumbers(ProcName p) {
  return pkbInstance->getProcedureStatementNumbers(p);
}

std::vector<std::pair<StmtNum, ProcName>> ReadPKB::getCallStatements() {
  return pkbInstance->getCallStatements();
}

std::unordered_set<ProcName> ReadPKB::getAllProcedureNames() {
  return pkbInstance->getAllProcedureNames();
}

std::unordered_set<Ent> ReadPKB::getUsesS(StmtNum num) {
  return pkbInstance->getUsesS(num);
}

std::unordered_set<Ent> ReadPKB::getUsesP(ProcName name) {
  return pkbInstance->getUsesP(name);
}

std::unordered_set<Ent> ReadPKB::getModifiesS(StmtNum num) {
  return pkbInstance->getModifiesS(num);
}

std::unordered_set<Ent> ReadPKB::getModifiesP(ProcName name) {
  return pkbInstance->getModifiesP(name);
}

std::unordered_set<StmtNum> ReadPKB::getIfStatementNumbers() {
  return pkbInstance->getIfStatementNumbers();
}

std::unordered_set<StmtNum> ReadPKB::getWhileStatementNumbers() {
  return pkbInstance->getWhileStatementNumbers();
}

std::unordered_set<StmtNum>
ReadPKB::getContainedStatements(StmtNum containerNum) {
  return pkbInstance->getContainedStatements(containerNum);
}

std::pair<StmtNum, ProcName> ReadPKB::getCallStmt(StmtNum s) {
  return pkbInstance->getCallStmt(s);
}

CFGNodeStub *ReadPKB::getCFG(StmtNum num) { return pkbInstance->getCFG(num); }

// TODO: liaise with QPS on what they want as return type for the below
std::vector<StmtNum> ReadPKB::getNextRHS(StmtNum n1) {
  return pkbInstance->getNextRHS(n1);
}

std::vector<StmtNum> ReadPKB::getNextLHS(StmtNum n2) {
  return pkbInstance->getNextLHS(n2);
}

std::vector<StmtNum> ReadPKB::getNextTRHS(StmtNum n1) {
  return pkbInstance->getNextTRHS(n1);
}

std::vector<StmtNum> ReadPKB::getNextTLHS(StmtNum n2) {
  return pkbInstance->getNextTLHS(n2);
}
