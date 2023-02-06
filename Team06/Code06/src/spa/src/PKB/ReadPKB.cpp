#include "ReadPKB.h"

void ReadPKB::setInstancePKB(PKB &pkb) {
    if (this -> pkbInstance != NULL) {
        return;
    }
    this -> pkbInstance = &pkb;
    return;
}

bool ReadPKB::checkFollows(StmtNum left, StmtNum right) {
    return pkbInstance -> followsApi->checkFollows(left, right);
}

StmtNum ReadPKB::getFollower(StmtNum followee) {
    return pkbInstance -> followsApi->getFollower(followee);
}

StmtNum ReadPKB::getFollowee(StmtNum follower) {
    return pkbInstance -> followsApi->getFollowee(follower);
}

bool ReadPKB::checkFollowsT(StmtNum followee, StmtNum follower) {
    return pkbInstance -> followsTApi->checkFollowsT(followee, follower);
}

std::unordered_set<StmtNum> ReadPKB::getFollowersT(StmtNum followee) {
    return pkbInstance -> followsTApi->getFollowersT(followee);
}

std::unordered_set<StmtNum> ReadPKB::getFolloweesT(StmtNum follower) {
    return pkbInstance -> followsTApi->getFolloweesT(follower);
}

bool ReadPKB::checkParent(StmtNum parent, StmtNum child) {
    return pkbInstance->parentApi->checkParent(parent, child);
}

std::vector<StmtNum> ReadPKB::getChildren(StmtNum parent) {
    return pkbInstance->parentApi->getChildren(parent);
}

StmtNum ReadPKB::getParent(StmtNum child) {
    return pkbInstance->parentApi->getParent(child);
}

bool ReadPKB::checkStatement(Stmt stmt, StmtNum num){
    return pkbInstance->statementApi->checkStatement(stmt, num);
}

std::unordered_set<StmtNum> ReadPKB::getStatementNumbers(Stmt stmt) {
    return pkbInstance->statementApi->getStatementNumbers(stmt);
}

bool ReadPKB::checkEntity(Ent e, StmtNum num) {
    return pkbInstance->entityStorage->checkEntity(e, num);
}

std::unordered_set<StmtNum> ReadPKB::getEntityStatementNumbers(Ent e) {
    return pkbInstance->entityStorage->getEntityStmtNums(e);
}

bool ReadPKB::checkProcedure(Proc p, StmtNum num) {
    return pkbInstance->procedureStorage->checkProcedure(p, num);
}

std::unordered_set<StmtNum> ReadPKB::getProcedureStatementNumbers(Proc p) {
    return pkbInstance->procedureStorage->getProcedureStatementNumbers(p);
}


bool ReadPKB::checkConstant(Const c, StmtNum num) {
    return pkbInstance->constantStorage->checkConstant(c, num);
}

std::unordered_set<StmtNum> ReadPKB::getConstantStatementNumbers(Const c) {
    return pkbInstance->constantStorage->getConstantStmtNums(c);
}

std::vector<StmtNum> ReadPKB::interpretQuery(QueryStub qs) {
    return pkbInstance->patternStorage->interpretQuery(qs);
}
