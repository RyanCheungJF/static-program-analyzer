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

CFGNodeStub* ReadPKB::getCFG(StmtNum num) {
    return pkbInstance->cfgStorage->getNode(num);
}

//TODO: liaise with QPS on what they want as return type for the below
std::vector<StmtNum> ReadPKB::getNextRHS(StmtNum n1) {
    CFGNodeStub* node = pkbInstance->cfgStorage->getNode(n1);
    if (node == nullptr) {
        return {};
    }
    if ((n1 + 1) <= (node->last)) {
        return {n1 + 1};
    }
    std::vector<StmtNum> res;
    std::vector<CFGNodeStub*> children = node->children;
    for (CFGNodeStub* child : children) {
        res.push_back(child->first);
    }
    return res;
}

std::vector<StmtNum> ReadPKB::getNextLHS(StmtNum n2) {
    CFGNodeStub* node = pkbInstance->cfgStorage->getNode(n2);
    if (node == nullptr) {
        return {};
    }
    if ((n2 - 1) >= (node->first)) {
        return {n2 - 1};
    }

    std::vector<StmtNum> res;
    for (auto child : node->parents) {
        res.push_back(child->last);
    }
    return res;
}

std::vector<StmtNum> ReadPKB::getNextTRHS(StmtNum n1) {
    CFGNodeStub* node = pkbInstance->cfgStorage->getNode(n1);
    if (node == nullptr) {
        return {};
    }

    std::unordered_set<StmtNum> res;
    std::queue<CFGNodeStub*> queue;

    for (int i = n1; i <= node->last; i++) {
        res.insert(i);
    }
    for (CFGNodeStub* child : node->children) {
        queue.push(child);
    }

    std::unordered_set<StmtNum> seen;
    while (!queue.empty()) {
        CFGNodeStub* curr = queue.front();
        queue.pop();

        if (seen.find(curr->first) != seen.end()) {
            continue;
        }
        seen.insert(curr->first);

        for (int i = curr->first; i <= curr->last; i++) {
            res.insert(i);
        }

        for (CFGNodeStub* child : curr->children) {
            queue.push(child);
        }
    }
    std::vector<StmtNum> result;
    result.insert(result.end(), res.begin(), res.end());
    return result;
}

std::vector<StmtNum> ReadPKB::getNextTLHS(StmtNum n2) {
    CFGNodeStub* node = pkbInstance->cfgStorage->getNode(n2);
    if (node == nullptr) {
        return {};
    }

    std::unordered_set<StmtNum> res;
    std::queue<CFGNodeStub*> queue;

    for (int i = n2; i >= node->first; i--) {
        res.insert(i);
    }
    for (CFGNodeStub* parent : node->parents) {
        queue.push(parent);
    }

    std::unordered_set<StmtNum> seen;
    while (!queue.empty()) {
        CFGNodeStub* curr = queue.front();
        queue.pop();

        if (seen.find(curr->last) != seen.end()) {
            continue;
        }
        seen.insert(curr->last);

        for (int i = curr->last; i >= curr->first; i--) {
            res.insert(i);
        }

        for (CFGNodeStub* parent : curr->parents) {
            queue.push(parent);
        }
    }
    std::vector<StmtNum> result;
    result.insert(result.end(), res.begin(), res.end());
    return result;
}

