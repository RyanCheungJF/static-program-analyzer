#include "NextTHandler.h"

NextTHandler::NextTHandler(std::shared_ptr<CFGStorage> cfgStorage, std::shared_ptr<StmtStorage> stmtStorage, std::shared_ptr<ProcedureStorage> procStorage) {
    this->cfgStorage = cfgStorage;
    this->stmtStorage = stmtStorage;
    this->procStorage = procStorage;
}

std::vector<std::vector<std::string>> NextTHandler::handle(Parameter param1, Parameter param2) {

    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    ParameterType paramType1 = param1.getType();
    ParameterType paramType2 = param2.getType();

    bool isFixedIntParam1 = paramType1 == ParameterType::FIXED_INT;
    bool isFixedIntParam2 = paramType2 == ParameterType::FIXED_INT;
    bool isWildCardParam1 = paramType1 == ParameterType::WILDCARD || paramType1 == ParameterType::STMT;
    bool isWildCardParam2 = paramType2 == ParameterType::WILDCARD || paramType2 == ParameterType::STMT;
    bool isTypedStmtParam1 = stmtTypesSet.find(paramType1) != stmtTypesSet.end();
    bool isTypedStmtParam2 = stmtTypesSet.find(paramType2) != stmtTypesSet.end();

    if (isFixedIntParam1) {
        if (isFixedIntParam2) {
            return handleIntInt(param1, param2);
        }
        else if (isTypedStmtParam2) {
            return handleIntStmttype(param1, param2);
        }
        else if (isWildCardParam2) {
            return handleIntWildcard(param1);
        }
    }
    else if (isTypedStmtParam1) {
        if (isFixedIntParam2) {
            return handleStmttypeInt(param1, param2);
        }
        else if (isTypedStmtParam2) {
            return handleStmttypeStmttype(param1, param2);
        }
        else if (isWildCardParam2) {
            return handleStmttypeWildcard(param1);
        }
    }
    else if (isWildCardParam1) {
        if (isFixedIntParam2) {
            return handleWildcardInt(param2);
        }
        else if (isTypedStmtParam2) {
            return handleWildcardStmttype(param2);
        }
        else if (isWildCardParam2) {
            return handleWildcardWildcard();
        }
    }
    return std::vector<std::vector<std::string>>();
}

// returns {lineNum, lineNum}
std::vector<std::vector<std::string>> NextTHandler::handleIntInt(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    ProcName proc1 = procStorage->getProcedure(stoi(paramString1));
    ProcName proc2 = procStorage->getProcedure(stoi(paramString2));
    std::vector<std::vector<std::string>> res;

    if (proc1 == "INVALID" || proc2 == "INVALID") {
        return res;
    } else if (proc1 != proc2) {
        return res;
    }

    std::deque<std::vector<StmtNum>> queue;
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc1);
    for (StmtNum child : graph[stoi(paramString1)][AppConstants::CHILDREN]) {
        if (child == stoi(paramString2)) {
            res.push_back({paramString1, paramString2});
            return res;
        }
        queue.push_back({stoi(paramString1), child});
    }

    std::unordered_set<StmtNum> seen;
    while (!queue.empty()) {
        std::vector<StmtNum> curr = queue.front();
        queue.pop_front();

        if (seen.find(curr[1]) != seen.end()) {
            continue;
        }
        seen.insert(curr[1]);

        if (curr[1] == stoi(paramString2)) {
            res.push_back({paramString1, paramString2});
            return res;
        }

        for (StmtNum child : graph[curr[1]][AppConstants::CHILDREN]) {
            queue.push_back({curr[0], child});
        }
    }
    return res;
}

// returns {param1Num, sNum}
std::vector<std::vector<std::string>> NextTHandler::handleIntWildcard(Parameter param1) {
    std::string paramString1 = param1.getValue();
    ProcName proc1 = procStorage->getProcedure(stoi(paramString1));
    std::vector<std::vector<std::string>> res;
    if (proc1 == "INVALID") {
        return res;
    }

    std::deque<std::vector<StmtNum>> queue;
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc1);
    for (StmtNum child : graph[stoi(paramString1)][AppConstants::CHILDREN]) {
        queue.push_back({stoi(paramString1), child});
    }

    std::unordered_set<StmtNum> seen;
    while (!queue.empty()) {
        std::vector<StmtNum> curr = queue.front();
        queue.pop_front();

        if (seen.find(curr[1]) != seen.end()) {
            continue;
        }
        seen.insert(curr[1]);
        res.push_back({std::to_string(curr[0]), std::to_string(curr[1])});

        for (StmtNum child : graph[curr[1]][AppConstants::CHILDREN]) {
            queue.push_back({curr[0], child});
        }
    }
    return res;
}

// returns {sNum, param2Num}
std::vector<std::vector<std::string>> NextTHandler::handleWildcardInt(Parameter param2) {
    std::string paramString2 = param2.getValue();
    ProcName proc = procStorage->getProcedure(stoi(paramString2));
    std::vector<std::vector<std::string>> res;
    if (proc == "INVALID") {
        return res;
    }

    std::deque<std::vector<StmtNum>> queue;
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc);
    for (StmtNum parent : graph[stoi(paramString2)][AppConstants::PARENTS]) {
        queue.push_back({parent, stoi(paramString2)});
    }

    std::unordered_set<StmtNum> seen;
    while (!queue.empty()) {
        std::vector<StmtNum> curr = queue.front();
        queue.pop_front();

        if (seen.find(curr[0]) != seen.end()) {
            continue;
        }
        seen.insert(curr[0]);
        res.push_back({std::to_string(curr[0]), std::to_string(curr[1])});

        for (StmtNum parent : graph[curr[1]][AppConstants::PARENTS]) {
            queue.push_back({parent, curr[1]});
        }
    }
    return res;
}

// returns {param1Num, lineNum}
std::vector<std::vector<std::string>> NextTHandler::handleStmttypeInt(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    Stmt type = param1.getTypeString();
    ProcName proc = procStorage->getProcedure(stoi(paramString2));
    std::vector<std::vector<std::string>> res;

    if (proc == "INVALID") {
        return res;
    }

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::deque<std::vector<StmtNum>> queue;
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc);
    for (StmtNum parent : graph[stoi(paramString2)][AppConstants::PARENTS]) {
        queue.push_back({parent, stoi(paramString2)});
    }

    std::unordered_set<StmtNum> seen;
    while (!queue.empty()) {
        std::vector<StmtNum> curr = queue.front();
        queue.pop_front();

        if (seen.find(curr[0]) != seen.end()) {
            continue;
        }
        seen.insert(curr[0]);

        if (stmttypeLines.find(curr[0]) != stmttypeLines.end()) {
            res.push_back({std::to_string(curr[0]), std::to_string(curr[1])});
        }

        for (StmtNum parent : graph[curr[1]][AppConstants::PARENTS]) {
            queue.push_back({parent, curr[1]});
        }
    }
    return res;
}

// returns {lineNum, param2Num}
std::vector<std::vector<std::string>> NextTHandler::handleIntStmttype(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    Stmt type = param2.getTypeString();
    ProcName proc = procStorage->getProcedure(stoi(paramString1));
    std::vector<std::vector<std::string>> res;

    if (proc == "INVALID") {
        return res;
    }

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::deque<std::vector<StmtNum>> queue;
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc);
    for (StmtNum child : graph[stoi(paramString1)][AppConstants::CHILDREN]) {
        queue.push_back({stoi(paramString1), child});
    }

    std::unordered_set<StmtNum> seen;
    while (!queue.empty()) {
        std::vector<StmtNum> curr = queue.front();
        queue.pop_front();

        if (seen.find(curr[1]) != seen.end()) {
            continue;
        }
        seen.insert(curr[1]);
        if (stmttypeLines.find(curr[1]) != stmttypeLines.end()) {
            res.push_back({std::to_string(curr[0]), std::to_string(curr[1])});
        }

        for (StmtNum child : graph[curr[1]][AppConstants::CHILDREN]) {
            queue.push_back({curr[0], child});
        }
    }
    return res;
}

// returns {param1Num, sNum}
std::vector<std::vector<std::string>> NextTHandler::handleStmttypeWildcard(Parameter param1) {
    std::string paramString1 = param1.getValue();
    Stmt type = param1.getTypeString();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines = getProcedureLines(stmttypeLines);

    for (auto kv : procedure_lines) {
        ProcName proc = kv.first;
        std::unordered_set<StmtNum> lines = kv.second;

        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc);
        for (StmtNum line : lines) {

            std::deque<std::vector<StmtNum>> queue;
            std::unordered_set<StmtNum> seen;
            std::unordered_set<StmtNum> children = graph[line][AppConstants::CHILDREN];
            for (StmtNum child : children) {
                queue.push_back({line, child});
            }

            while (!queue.empty()) {
                std::vector<StmtNum> curr = queue.front();
                queue.pop_front();
                if (seen.find(curr[1]) != seen.end()) {
                    continue;
                }

                seen.insert(curr[1]);
                res.push_back({std::to_string(curr[0]), std::to_string(curr[1])});

                for (StmtNum child : graph[curr[1]][AppConstants::CHILDREN]) {
                    queue.push_back({curr[0], child});
                }
            }
        }
    }
    return res;
}

// returns {sNum, param2Num}
std::vector<std::vector<std::string>> NextTHandler::handleWildcardStmttype(Parameter param2) {
    std::string paramString1 = param2.getValue();
    Stmt type = param2.getTypeString();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines = getProcedureLines(stmttypeLines);

    for (auto kv : procedure_lines) {
        ProcName proc = kv.first;
        std::unordered_set<StmtNum> lines = kv.second;

        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc);
        for (StmtNum line : lines) {

            std::deque<std::vector<StmtNum>> queue;
            std::unordered_set<StmtNum> seen;
            std::unordered_set<StmtNum> parents = graph[line][AppConstants::PARENTS];
            for (StmtNum parent : parents) {
                queue.push_back({parent, line});
            }

            while (!queue.empty()) {
                std::vector<StmtNum> curr = queue.front();
                queue.pop_front();
                if (seen.find(curr[0]) != seen.end()) {
                    continue;
                }

                seen.insert(curr[0]);
                res.push_back({std::to_string(curr[0]), std::to_string(curr[1])});

                for (StmtNum parent : graph[curr[1]][AppConstants::PARENTS]) {
                    queue.push_back({parent, curr[1]});
                }
            }
        }
    }
    return res;
}

std::vector<std::vector<std::string>> NextTHandler::handleStmttypeStmttype(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    Stmt type = param1.getTypeString();
    Stmt type2 = param2.getTypeString();
    std::unordered_set<StmtNum> stmttypeLines2 = stmtStorage->getStatementNumbers(type2);
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines = getProcedureLines(stmttypeLines);

    for (auto kv : procedure_lines) {
        ProcName proc = kv.first;
        std::unordered_set<StmtNum> lines = kv.second;

        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc);
        for (StmtNum line : lines) {

            std::deque<std::vector<StmtNum>> queue;
            std::unordered_set<StmtNum> seen;
            std::unordered_set<StmtNum> children = graph[line][AppConstants::CHILDREN];
            for (StmtNum child : children) {
                queue.push_back({line, child});
            }

            while (!queue.empty()) {
                std::vector<StmtNum> curr = queue.front();
                queue.pop_front();
                if (seen.find(curr[1]) != seen.end()) {
                    continue;
                }

                seen.insert(curr[1]);
                if (stmttypeLines2.find(curr[1]) != stmttypeLines2.end()) {
                    res.push_back({std::to_string(curr[0]), std::to_string(curr[1])});
                }

                for (StmtNum child : graph[curr[1]][AppConstants::CHILDREN]) {
                    queue.push_back({curr[0], child});
                }
            }
        }
    }
    return res;
}

std::vector<std::vector<std::string>> NextTHandler::handleWildcardWildcard() {
    std::unordered_set<ProcName> procedures = procStorage->getProcNames();
    std::vector<std::vector<std::string>> res;

    for (ProcName proc : procedures) {
        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc);

        for (auto kv : graph) {
            StmtNum parent = kv.first;

            std::deque<std::vector<StmtNum>> queue;
            std::unordered_set<StmtNum> seen;
            auto children = graph[parent][AppConstants::CHILDREN];
            for (StmtNum child : children) {
                queue.push_back({parent, child});
            }

            while (!queue.empty()) {
                std::vector<StmtNum> curr = queue.front();
                queue.pop_front();
                if (seen.find(curr[1]) != seen.end()) {
                    continue;
                }

                seen.insert(curr[1]);
                res.push_back({std::to_string(curr[0]), std::to_string(curr[1])});

                for (StmtNum child : graph[curr[1]][AppConstants::CHILDREN]) {
                    queue.push_back({curr[0], child});
                }
            }
        }
    }
    return res;
}



// helper functions
std::unordered_map<ProcName, std::unordered_set<StmtNum>> NextTHandler::getProcedureLines(std::unordered_set<StmtNum> statementNumbers) {
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines;
    for (StmtNum num : statementNumbers) {
        ProcName proc = procStorage->getProcedure(num);
        procedure_lines[proc].insert(num);
    }
    return procedure_lines;
};
