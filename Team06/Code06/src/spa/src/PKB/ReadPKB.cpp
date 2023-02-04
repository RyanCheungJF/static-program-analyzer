#include "ReadPKB.h"

void ReadPKB::setInstancePKB(PKB &pkb) {
    if (this -> pkbInstance != NULL) {
        return;
    }
    this -> pkbInstance = &pkb;
    this->stmtStmtHandlerMap[RelationshipType::FOLLOWS] = pkb.followsStorage;
    return;
}

std::vector<std::pair<std::string, std::string>> ReadPKB::findRelationship(Relationship rs) {
    RelationshipType type = rs.type;
    std::string param1 = rs.params[0].getValue();
    std::string param2 = rs.params[1].getValue();
    if (stmtStmtHandlerMap.find(type) != stmtStmtHandlerMap.end()) {
        StmtStmtRLHandler handler;
        return handler.handle(stmtStmtHandlerMap.at(type), param1, param2);
    } else if (stmtEntHandlerMap.find(type) != stmtEntHandlerMap.end()) {
        StmtEntRLHandler handler;
        return handler.handle(stmtEntHandlerMap.at(type));
    } else if (entEntHandlerMap.find(type) != entEntHandlerMap.end()) {
        EntEntRLHandler handler;
        return handler.handle(entEntHandlerMap.at(type));
    }
    return std::vector<std::pair<std::string, std::string>>();
}

//TODO Needs to handle Procedures 
std::vector<std::string> ReadPKB::findDesignEntities(Parameter p) {
    std::vector<std::string> res;
    std::string typeString = p.typeToString(p.getType());
    if (p.isStatementRef(p)) {
        std::unordered_set stmtNums = pkbInstance->statementStorage->getStatementNumbers(typeString);
        for (auto stmtNum : stmtNums) {
            res.push_back(to_string(stmtNum));
        }
    }
    else if (p.isEntityRef(p)) {
        std::unordered_set vars = pkbInstance->entityStorage->getEntNames(typeString);
        for (auto var : vars) {
            res.push_back(var);
        }
    }
  
    return res;
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
    return pkbInstance->statementStorage->checkStatement(stmt, num);
}

std::unordered_set<StmtNum> ReadPKB::getStatementNumbers(Stmt stmt) {
    return pkbInstance->statementStorage->getStatementNumbers(stmt);
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

