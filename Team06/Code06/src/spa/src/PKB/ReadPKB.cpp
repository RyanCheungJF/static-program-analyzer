#include "ReadPKB.h"

void ReadPKB::setInstancePKB(PKB &pkb) {
    if (this -> pkbInstance != NULL) {
        return;
    }
    this -> pkbInstance = &pkb;
    this->stmtStmtHandlerMap[RelationshipType::FOLLOWS] = pkb.followsStorage;
    return;
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
    
    /*std::stringstream ss;
    for (int i = 0; i < rhs.length(); i++) {
        char curr = rhs[i];
        if (curr != '_') {
            ss << curr;
        }
    }*/

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

std::unordered_set<StmtNum> ReadPKB::getProcedureStatementNumbers(Proc p) {
    return pkbInstance->procedureStorage->getProcedureStatementNumbers(p);
}

std::vector<std::pair<StmtNum, ProcName>> ReadPKB::getCallStatements() {
    return pkbInstance->callStorage->getCallStatements();
}

std::unordered_set<ProcName> ReadPKB::getAllProcedureNames() {
    return pkbInstance->procedureStorage->getProcNames();
}

std::unordered_set<Ent> ReadPKB::getUsesS(StmtNum num) {
    return pkbInstance->usesStorage->getUsesS(num);
}


