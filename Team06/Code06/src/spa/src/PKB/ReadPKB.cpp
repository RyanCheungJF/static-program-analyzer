#include "ReadPKB.h"

void ReadPKB::setInstancePKB(PKB& pkb) {
    if (this->pkbInstance != NULL) {
        return;
    }
    this->pkbInstance = &pkb;
}

std::vector<std::vector<std::string>> ReadPKB::findRelationship(shared_ptr<Relationship> rs) {
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

std::unordered_set<StmtNum>& ReadPKB::getProcedureStatementNumbers(ProcName p) {
    return pkbInstance->getProcedureStatementNumbers(p);
}

std::unordered_set<ProcName>& ReadPKB::getAllProcedureNames() {
    return pkbInstance->getAllProcedureNames();
}

std::unordered_set<Ent>* ReadPKB::getUsesS(StmtNum num) {
    return pkbInstance->getUsesS(num);
}

std::unordered_set<Ent>* ReadPKB::getUsesP(ProcName name) {
    return pkbInstance->getUsesP(name);
}

std::unordered_set<Ent>* ReadPKB::getModifiesS(StmtNum num) {
    return pkbInstance->getModifiesS(num);
}

std::unordered_set<Ent>* ReadPKB::getModifiesP(ProcName name) {
    return pkbInstance->getModifiesP(name);
}

std::unordered_set<StmtNum>* ReadPKB::getIfStatementNumbers() {
    return pkbInstance->getIfStatementNumbers();
}

std::unordered_set<StmtNum>* ReadPKB::getWhileStatementNumbers() {
    return pkbInstance->getWhileStatementNumbers();
}

std::unordered_set<StmtNum>* ReadPKB::getContainedStatements(StmtNum containerNum) {
    return pkbInstance->getContainedStatements(containerNum);
}

std::pair<StmtNum, ProcName> ReadPKB::getCallStmt(StmtNum s) {
    return pkbInstance->getCallStmt(s);
}

std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>*
ReadPKB::getCFG(ProcName name) {
    return pkbInstance->getCFG(name);
}

std::unordered_set<ProcName>* ReadPKB::getCallsT(ProcName p) {
    return pkbInstance->getCallsT(p);
}
