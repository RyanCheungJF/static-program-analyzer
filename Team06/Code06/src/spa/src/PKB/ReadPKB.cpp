#include "ReadPKB.h"

void ReadPKB::setInstancePKB(PKB &pkb) {
    if (this -> pkbInstance != NULL) {
        return;
    }
    this -> pkbInstance = &pkb;
    this->stmtStmtHandlerMap[RelationshipType::FOLLOWS] = pkb.followsStorage;
    return;
}

std::vector<std::vector<std::string>> ReadPKB::findRelationship(Relationship rs) {
    RelationshipType type = rs.type;
    Parameter param1 = rs.params[0];
    Parameter param2 = rs.params[1];
    if (stmtStmtHandlerMap.find(type) != stmtStmtHandlerMap.end()) {
        StmtStmtRLHandler handler(stmtStmtHandlerMap.at(type), pkbInstance->statementStorage);
        return handler.handle(param1, param2);
    } else if (stmtEntHandlerMap.find(type) != stmtEntHandlerMap.end()) {
        StmtEntRLHandler handler;
        return handler.handle(stmtEntHandlerMap.at(type));
    } else if (entEntHandlerMap.find(type) != entEntHandlerMap.end()) {
        EntEntRLHandler handler;
        return handler.handle(entEntHandlerMap.at(type));
    }
    return std::vector<std::vector<std::string>>();
}

std::vector<std::string> ReadPKB::findDesignEntities(Parameter p) {
    std::vector<std::string> res;
    std::string typeString = p.getTypeString();
    if (p.getType() == ParameterType::PROCEDURE) {
        std::unordered_set<Proc> procs = pkbInstance->procedureStorage->getProcNames();
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
    Parameter* lhs = p.getEntRef();
    std::string rhs = p.pattern;
    std::string lhsString = lhs->getValue();
    ParameterType lhsType = lhs->getType();
    bool leftWildcard = false;
    bool rightWildcard = false;

    if (rhs != "_") {
        leftWildcard = rhs[0] == '_';
        rightWildcard = rhs[rhs.length() - 1] == '_';
    }

    if (leftWildcard && rightWildcard) rhs = rhs.substr(1, rhs.length() - 2);

    if (lhsType == ParameterType::FIXED_STRING) {
        if (rhs == "_") {
            std::cout << "hitting getMatchingLHS ie pattern(\"a\", _) for " << lhsString << " = " << rhs << "\n";
            return pkbInstance->patternStorage->getMatchingLHS(lhsString);
        }
        else if (leftWildcard && rightWildcard) {
            std::cout << "hitting getMatchingRHSBothWildcard ie pattern(\"a\", _\"x\"_) for " << lhsString << " = " << rhs << "\n";
            return pkbInstance->patternStorage->getMatchingRHSBothWildcard(lhsString, rhs);
        }
        else {
            std::cout << "hitting getMatchingExact ie pattern(\"a\", \"x\") for " << lhsString << " = " << rhs << "\n";
            return pkbInstance->patternStorage->getMatchingExact(lhsString, rhs);
        }
    }
    else if (lhsType == ParameterType::VARIABLE) {
        if (rhs == "_") {
            std::cout << "hitting getMatchingLHS ie pattern(v, _) for " << lhsString << " = " << rhs << "\n";
            return pkbInstance->patternStorage->getLHSAndStmtNum();
        }
        else if (leftWildcard && rightWildcard) {
            std::cout << "hitting getMatchingRHSBothWildcard ie pattern(v, _\"x\"_) for " << lhsString << " = " << rhs << "\n";
            return pkbInstance->patternStorage->getLHSAndStmtNumRHSBothWildcard(rhs);
        }
        else {
            std::cout << "hitting getMatchingExact ie pattern(v, \"x\") for " << lhsString << " = " << rhs << "\n";
            return pkbInstance->patternStorage->getLHSAndStmtNumRHSNoWildcard(rhs);
        }
    }
    else {
        if (rhs == "_") {
            std::cout << "hitting getMatchingLHS ie pattern(_, _) for " << lhsString << " = " << rhs << "\n";
            return pkbInstance->patternStorage->getLHSAndStmtNum();
        }
        else if (leftWildcard && rightWildcard) {
            std::cout << "hitting getMatchingRHSBothWildcard ie pattern(_, _\"x\"_) for " << lhsString << " = " << rhs << "\n";
            return pkbInstance->patternStorage->getMatchingLHSWildcardRHSBothWildcard(rhs);
        }
        else {
            std::cout << "hitting getMatchingExact ie pattern(_, \"x\") for " << lhsString << " = " << rhs << "\n";
            return pkbInstance->patternStorage->getMatchingLHSWildcardRHSNoWildcard(rhs);
        }
    }
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


bool ReadPKB::checkConstant(Const c, StmtNum num) {
    return pkbInstance->constantStorage->checkConstant(c, num);
}

std::unordered_set<StmtNum> ReadPKB::getConstantStatementNumbers(Const c) {
    return pkbInstance->constantStorage->getConstantStmtNums(c);
}

std::unordered_set<Ent> ReadPKB::getUsesProcedure(ProcedureName name) {
    return pkbInstance->usesStorage->getUsesProcedure(name);
}

std::unordered_set<Ent> ReadPKB::getUsesStmtnum(StmtNum num) {
    return pkbInstance->usesStorage->getUsesStmtnum(num);
}

std::unordered_set<Ent> ReadPKB::getModifiesProcedure(ProcedureName name) {
    return pkbInstance->modifiesStorage->getModifiesProcedure(name);
}

std::unordered_set<Ent> ReadPKB::getModifiesStmtnum(StmtNum num) {
    return pkbInstance->modifiesStorage->getModifiesStmtnum(num);
}

std::unordered_set<StmtNum> ReadPKB::getFollowsRightWildcard(StmtNum followee) {
    return pkbInstance->followsStorage->getRightWildcard(followee);
}

std::unordered_set<StmtNum> ReadPKB::getFollowsLeftWildcard(StmtNum follower) {
    return pkbInstance->followsStorage->getLeftWildcard(follower);
}

std::pair<std::vector<StmtNum>, std::vector<StmtNum>> ReadPKB::getFollowsAllPairs() {
    return pkbInstance->followsStorage->getAllPairs();
}

std::unordered_set<StmtNum> ReadPKB::getFollowsTRightWildcard(StmtNum followee) {
    return pkbInstance->followsTStorage->getRightWildcard(followee);
}

std::unordered_set<StmtNum> ReadPKB::getFollowsTLeftWildcard(StmtNum follower) {
    return pkbInstance->followsTStorage->getLeftWildcard(follower);
}

std::pair<std::vector<StmtNum>, std::vector<StmtNum>> ReadPKB::getFollowsTAllPairs() {
    return pkbInstance->followsTStorage->getAllPairs();

}

std::unordered_set<StmtNum> ReadPKB::getParentRightWildcard(StmtNum parent) {
    return pkbInstance->parentStorage->getRightWildcard(parent);
}

std::unordered_set<StmtNum> ReadPKB::getParentLeftWildcard(StmtNum child) {
    return pkbInstance->parentStorage->getLeftWildcard(child);
}

std::pair<std::vector<StmtNum>, std::vector<StmtNum>> ReadPKB::getParentAllPairs() {
    return pkbInstance->parentStorage->getAllPairs();
}

std::unordered_set<StmtNum> ReadPKB::getParentTRightWildcard(StmtNum parent) {
    return pkbInstance->parentTStorage->getRightWildcard(parent);
}

std::unordered_set<StmtNum> ReadPKB::getParentTLeftWildcard(StmtNum child) {
    return pkbInstance->parentTStorage->getLeftWildcard(child);
}

std::pair<std::vector<StmtNum>, std::vector<StmtNum>> ReadPKB::getParentTAllPairs() {
    return pkbInstance->parentTStorage->getAllPairs();
}

