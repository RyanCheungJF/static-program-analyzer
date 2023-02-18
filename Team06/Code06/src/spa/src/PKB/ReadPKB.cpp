#include "ReadPKB.h"

void ReadPKB::setInstancePKB(PKB& pkb) {
    if (this->pkbInstance != NULL) {
        return;
    }
    this->pkbInstance = &pkb;

    this->followsParentMap[RelationshipType::FOLLOWS] = pkb.followsStorage;
    this->followsParentMap[RelationshipType::FOLLOWST] = pkb.followsTStorage;
    this->followsParentMap[RelationshipType::PARENT] = pkb.parentStorage;
    this->followsParentMap[RelationshipType::PARENTT] = pkb.parentTStorage;

    this->modifiesUsesMap[RelationshipType::MODIFIES] = pkb.modifiesStorage;
    this->modifiesUsesMap[RelationshipType::USES] = pkb.usesStorage;
    return;
}

std::vector<std::vector<std::string>> ReadPKB::findRelationship(shared_ptr<Relationship> rs) {
    RelationshipType type = rs->getType();
    vector<Parameter> params = rs->getParameters();
    Parameter param1 = params[0];
    Parameter param2 = params[1];
    if (followsParentMap.find(type) != followsParentMap.end()) {
        FollowsParentHandler handler(followsParentMap.at(type), pkbInstance->statementStorage);
        return handler.handle(param1, param2);

    } else if (modifiesUsesMap.find(type) != modifiesUsesMap.end()) {
        ModifiesUsesHandler handler(modifiesUsesMap.at(type), pkbInstance->statementStorage);

        return handler.handle(param1, param2);
    } 
    return std::vector<std::vector<std::string>>();
}

std::vector<std::string> ReadPKB::findDesignEntities(Parameter p) {
    std::vector<std::string> res;
    std::string typeString = p.getTypeString();
    if (p.getType() == ParameterType::PROCEDURE) {
        std::unordered_set<ProcName> procs = pkbInstance->procedureStorage->getProcNames();
        for (auto proc : procs) {
            res.push_back(proc);
        }
    }
    else if (p.getType() == ParameterType::CONSTANT) {
        std::unordered_set<Const> constants = pkbInstance->constantStorage->getConstNames();
        for (auto constant : constants) {
            res.push_back(to_string(constant));
        }
    }
    else if (p.getType() == ParameterType::VARIABLE) {
        std::unordered_set<Ent> vars = pkbInstance->entityStorage->getEntNames();
        for (auto var : vars) {
            res.push_back(var);
        }
    }
    else if (p.isStatementRef(p)) {
        std::unordered_set<StmtNum> stmtNums = pkbInstance->statementStorage->getStatementNumbers(typeString);
        for (auto stmtNum : stmtNums) {
            res.push_back(to_string(stmtNum));
        }
    }

    return res;
}

std::vector<std::vector<std::string>> ReadPKB::findPattern(Pattern p) {
    AssignPatternHandler handler(pkbInstance->patternStorage);
    return handler.handle(p);
}

bool ReadPKB::checkStatement(Stmt stmt, StmtNum num) {
    return pkbInstance->statementStorage->checkStatement(stmt, num);
}

std::unordered_set<StmtNum> ReadPKB::getProcedureStatementNumbers(ProcName p) {
    return pkbInstance->procedureStorage->getProcedureStatementNumbers(p);
}

std::vector<std::pair<StmtNum, ProcName>> ReadPKB::getCallStatements() {
    return pkbInstance->callStorage->getCallStatements();
}

std::unordered_set<ProcName> ReadPKB::getAllProcedureNames() {
    return pkbInstance->procedureStorage->getProcNames();
}

std::unordered_set<Ent> ReadPKB::getUsesS(StmtNum num) {
    return pkbInstance->usesStorage->getEnt(num);
}

std::unordered_set<Ent> ReadPKB::getUsesP(ProcName name) {
    return pkbInstance->usesStorage->getEnt(name);
}

std::unordered_set<Ent> ReadPKB::getModifiesS(StmtNum num) {
    return pkbInstance->modifiesStorage->getEnt(num);
}

std::unordered_set<Ent> ReadPKB::getModifiesP(ProcName name) {
    return pkbInstance->modifiesStorage->getEnt(name);
}

std::unordered_set<StmtNum> ReadPKB::getIfStatementNumbers() {
    return pkbInstance->statementStorage->getStatementNumbers(IF);
}

std::unordered_set<StmtNum> ReadPKB::getWhileStatementNumbers() {
    return pkbInstance->statementStorage->getStatementNumbers(WHILE);
}

std::unordered_set<StmtNum> ReadPKB::getContainedStatements(StmtNum containerNum) {
    return pkbInstance->parentTStorage->getRightWildcard(containerNum);
}

std::pair<StmtNum, ProcName> ReadPKB::getCallStmt(StmtNum s) {
    return pkbInstance->callStorage->getCallStmt(s);
}