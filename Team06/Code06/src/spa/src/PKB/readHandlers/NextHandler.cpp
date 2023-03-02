#include "NextHandler.h"

NextHandler::NextHandler(std::shared_ptr<CFGStorage> cfgStorage, std::shared_ptr<StmtStorage> stmtStorage, std::shared_ptr<ProcedureStorage> procStorage) {
    this->cfgStorage = cfgStorage;
    this->stmtStorage = stmtStorage;
    this->procStorage = procStorage;
}

std::vector<std::vector<std::string>> NextHandler::handle(Parameter param1, Parameter param2) {

    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    ParameterType paramType1 = param1.getType();
    ParameterType paramType2 = param2.getType();

    if (paramType1 == ParameterType::FIXED_INT && paramType2 == ParameterType::FIXED_INT) {
        return handleIntInt(param1, param2);
    } else if ((paramType1 == ParameterType::FIXED_INT && paramType2 == ParameterType::WILDCARD) ||
            (paramType1 == ParameterType::FIXED_INT && paramType2 == ParameterType::STMT)) {
        return handleIntWildcard(param1);
    } else if ((paramType1 == ParameterType::WILDCARD && paramType2 == ParameterType::FIXED_INT) ||
            (paramType1 == ParameterType::STMT && paramType2 == ParameterType::FIXED_INT)) {
        return handleWildcardInt(param2);
    } else if ((stmtTypesSet.find(paramType1) != stmtTypesSet.end()) && paramType2 == ParameterType::FIXED_INT) {
        return handleStmttypeInt(param1, param2);
    } else if (paramType1 == ParameterType::FIXED_INT && (stmtTypesSet.find(paramType2) != stmtTypesSet.end())) {
        return handleIntStmttype(param1, param2);
    } else if (((stmtTypesSet.find(paramType1) != stmtTypesSet.end()) && paramType2 == ParameterType::WILDCARD) ||
            ((stmtTypesSet.find(paramType1) != stmtTypesSet.end()) && paramType2 == ParameterType::STMT)) {
        return handleStmttypeWildcard(param1);
    } else if ((paramType1 == ParameterType::WILDCARD && (stmtTypesSet.find(paramType2) != stmtTypesSet.end())) ||
            (paramType1 == ParameterType::STMT && (stmtTypesSet.find(paramType2) != stmtTypesSet.end()))) {
        return handleWildcardStmttype(param2);
    } else if ((stmtTypesSet.find(paramType1) != stmtTypesSet.end()) && (stmtTypesSet.find(paramType2) != stmtTypesSet.end())) {
        return handleStmttypeStmttype(param1, param2);
    } else if ((paramType1 == ParameterType::WILDCARD && paramType2 == ParameterType::WILDCARD) ||
            (paramType1 == ParameterType::STMT && paramType2 == ParameterType::STMT)) {
        return handleWildcardWildcard();
    }
    return std::vector<std::vector<std::string>>();
}

// returns {lineNum, lineNum}
std::vector<std::vector<std::string>> NextHandler::handleIntInt(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    ProcName proc1 = procStorage->getProcedure(stoi(paramString1));
    ProcName proc2 = procStorage->getProcedure(stoi(paramString2));
    std::vector<std::vector<std::string>> res;

    if (proc1 == "INVALID" or proc2 == "INVALID") {
        return res;
    } else if (proc1 != proc2) {
        return res;
    }

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc1);
    if (graph[stoi(paramString1)][AppConstants::CHILDREN].find(stoi(paramString2)) != graph[stoi(paramString1)][AppConstants::CHILDREN].end()) {
        res.push_back({paramString1, paramString2});
    }
    return res;
}

// returns {param1Num, sNum}
std::vector<std::vector<std::string>> NextHandler::handleIntWildcard(Parameter param1) {
    std::string paramString1 = param1.getValue();
    ProcName proc1 = procStorage->getProcedure(stoi(paramString1));
    std::vector<std::vector<std::string>> res;

    if (proc1 == "INVALID") {
        return res;
    }

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc1);
    std::unordered_set<StmtNum> children = graph[stoi(paramString1)][AppConstants::CHILDREN];
    for (StmtNum child : children) {
        res.push_back({paramString1, std::to_string(child)});
    }
    return res;
}

// returns {sNum, param2Num}
std::vector<std::vector<std::string>> NextHandler::handleWildcardInt(Parameter param2) {
    std::string paramString2 = param2.getValue();
    ProcName proc2 = procStorage->getProcedure(stoi(paramString2));
    std::vector<std::vector<std::string>> res;

    if (proc2 == "INVALID") {
        return res;
    }

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc2);
    std::unordered_set<StmtNum> parents = graph[stoi(paramString2)][AppConstants::PARENTS];
    for (StmtNum parent : parents) {
        res.push_back({std::to_string(parent), paramString2});
    }
    return res;
}


// returns {param1Num, lineNum}
std::vector<std::vector<std::string>> NextHandler::handleStmttypeInt(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    Stmt type = stmtTypesSet[param1.getType()];
    ProcName proc = procStorage->getProcedure(stoi(paramString2));
    std::vector<std::vector<std::string>> res;

    if (proc == "INVALID") {
        return res;
    }

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc);
    std::unordered_set<StmtNum> parents = graph[stoi(paramString2)][AppConstants::PARENTS];
    for (StmtNum parent : parents) {
        if (stmttypeLines.find(parent) != stmttypeLines.end()) {
            res.push_back({std::to_string(parent), paramString2});
        }
    }
    return res;
}

// returns {lineNum, param2Num}
std::vector<std::vector<std::string>> NextHandler::handleIntStmttype(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    Stmt type = stmtTypesSet[param2.getType()];
    ProcName proc = procStorage->getProcedure(stoi(paramString1));
    std::vector<std::vector<std::string>> res;

    if (proc == "INVALID") {
        return res;
    }

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc);
    std::unordered_set<StmtNum> children = graph[stoi(paramString2)][AppConstants::CHILDREN];
    for (StmtNum child : children) {
        if (stmttypeLines.find(child) != stmttypeLines.end()) {
            res.push_back({paramString1, std::to_string(child)});
        }
    }
    return res;
}

// returns {param1Num, sNum}
std::vector<std::vector<std::string>> NextHandler::handleStmttypeWildcard(Parameter param1) {
    std::string paramString1 = param1.getValue();
    Stmt type = stmtTypesSet[param1.getType()];
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines;
    for (StmtNum num : stmttypeLines) {
        ProcName proc = procStorage->getProcedure(num);
        procedure_lines[proc].insert(num);
    }

    for (auto kv : procedure_lines) {
        ProcName proc = kv.first;
        std::unordered_set<StmtNum> lines = kv.second;
        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc);
        for (StmtNum line : lines) {
            std::unordered_set<StmtNum> children = graph[line][AppConstants::CHILDREN];
            for (StmtNum child : children) {
                res.push_back({std::to_string(line), std::to_string(child)});
            }
        }
    }
    return res;
}

// returns {sNum, param2Num}
std::vector<std::vector<std::string>> NextHandler::handleWildcardStmttype(Parameter param2) {
    std::string paramString2 = param2.getValue();
    Stmt type = stmtTypesSet[param2.getType()];
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines;
    for (StmtNum num : stmttypeLines) {
        ProcName proc = procStorage->getProcedure(num);
        procedure_lines[proc].insert(num);
    }

    for (auto kv : procedure_lines) {
        ProcName proc = kv.first;
        std::unordered_set<StmtNum> lines = kv.second;
        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc);
        for (StmtNum line : lines) {
            std::unordered_set<StmtNum> parents = graph[line][AppConstants::PARENTS];
            for (StmtNum parent : parents) {
                res.push_back({std::to_string(parent), std::to_string(line)});
            }
        }
    }
    return res;
}

std::vector<std::vector<std::string>> NextHandler::handleStmttypeStmttype(Parameter param1, Parameter param2) {

}

std::vector<std::vector<std::string>> NextHandler::handleWildcardWildcard() {
    std::unordered_set<ProcName> procedures = procStorage->getProcNames();
    std::vector<std::vector<std::string>> res;

    for (ProcName proc : procedures) {
        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc);

        for (auto kv : graph) {
            StmtNum p = kv.first;
            auto children = graph[p][AppConstants::CHILDREN];
            for (StmtNum child : children) {
                res.push_back({std::to_string(p), std::to_string(child)});
            }
        }
    }
    return res;
}
