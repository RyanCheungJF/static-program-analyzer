#include "NextHandler.h"

NextHandler::NextHandler(std::shared_ptr<CFGStorage> cfgStorage, std::shared_ptr<StmtStorage> stmtStorage,
                         std::shared_ptr<ProcedureStorage> procStorage, bool isTransitive) {
    this->cfgStorage = cfgStorage;
    this->stmtStorage = stmtStorage;
    this->procStorage = procStorage;
    this->isTransitive = isTransitive;
}

std::vector<std::vector<std::string>> NextHandler::handle(Parameter param1, Parameter param2) {

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

    if (isTransitive) {
        return handleTransitive(param1, param2, isFixedIntParam1, isFixedIntParam2, isWildCardParam1, isWildCardParam2,
                                isTypedStmtParam1, isTypedStmtParam2);
    }
    return handleNonTransitive(param1, param2, isFixedIntParam1, isFixedIntParam2, isWildCardParam1, isWildCardParam2,
                               isTypedStmtParam1, isTypedStmtParam2);
}

// returns {lineNum, lineNum}
std::vector<std::vector<std::string>> NextHandler::handleIntInt(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    StmtNum n1 = stoi(paramString1);
    StmtNum n2 = stoi(paramString2);
    ProcName proc1 = procStorage->getProcedure(n1);
    ProcName proc2 = procStorage->getProcedure(n2);
    std::vector<std::vector<std::string>> res;

    if (proc1 == AppConstants::PROCEDURE_DOES_NOT_EXIST || proc2 == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }
    else if (proc1 != proc2) {
        return res;
    }

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
        cfgStorage->getGraph(proc1);
    if (graph[n1][AppConstants::CHILDREN].find(n2) != graph[n1][AppConstants::CHILDREN].end()) {
        res.push_back({paramString1, paramString2});
    }
    return res;
}

// returns {param1Num, sNum}
std::vector<std::vector<std::string>> NextHandler::handleIntWildcard(Parameter param1) {
    std::string paramString1 = param1.getValue();
    StmtNum n1 = stoi(paramString1);
    ProcName proc1 = procStorage->getProcedure(n1);
    std::vector<std::vector<std::string>> res;

    if (proc1 == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
        cfgStorage->getGraph(proc1);
    std::unordered_set<StmtNum> children = graph[n1][AppConstants::CHILDREN];
    for (StmtNum child : children) {
        res.push_back({paramString1, std::to_string(child)});
    }
    return res;
}

// returns {sNum, param2Num}
std::vector<std::vector<std::string>> NextHandler::handleWildcardInt(Parameter param2) {
    std::string paramString2 = param2.getValue();
    StmtNum n2 = stoi(paramString2);
    ProcName proc2 = procStorage->getProcedure(n2);
    std::vector<std::vector<std::string>> res;

    if (proc2 == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
        cfgStorage->getGraph(proc2);
    std::unordered_set<StmtNum> parents = graph[n2][AppConstants::PARENTS];
    for (StmtNum parent : parents) {
        res.push_back({std::to_string(parent), paramString2});
    }
    return res;
}

// returns {param1Num, lineNum}
std::vector<std::vector<std::string>> NextHandler::handleStmttypeInt(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    StmtNum n2 = stoi(paramString2);
    Stmt type = param1.getTypeString();
    ProcName proc = procStorage->getProcedure(n2);
    std::vector<std::vector<std::string>> res;

    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
        cfgStorage->getGraph(proc);
    std::unordered_set<StmtNum> parents = graph[n2][AppConstants::PARENTS];
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
    StmtNum n1 = stoi(paramString1);
    Stmt type = param2.getTypeString();
    ProcName proc = procStorage->getProcedure(n1);
    std::vector<std::vector<std::string>> res;

    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
        cfgStorage->getGraph(proc);
    std::unordered_set<StmtNum> children = graph[n1][AppConstants::CHILDREN];
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
    Stmt type = param1.getTypeString();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines = getProcedureLines(stmttypeLines);

    for (auto kv : procedure_lines) {
        ProcName proc = kv.first;
        std::unordered_set<StmtNum> lines = kv.second;
        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
            cfgStorage->getGraph(proc);
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
    Stmt type = param2.getTypeString();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines = getProcedureLines(stmttypeLines);

    for (auto kv : procedure_lines) {
        ProcName proc = kv.first;
        std::unordered_set<StmtNum> lines = kv.second;
        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
            cfgStorage->getGraph(proc);
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
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    Stmt type1 = param1.getTypeString();
    Stmt type2 = param2.getTypeString();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum> stmttypeLines1 = stmtStorage->getStatementNumbers(type1);
    std::unordered_set<StmtNum> stmttypeLines2 = stmtStorage->getStatementNumbers(type2);

    if (stmttypeLines1.size() < stmttypeLines2.size()) {
        std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines = getProcedureLines(stmttypeLines1);

        for (auto kv : procedure_lines) {
            ProcName proc = kv.first;
            std::unordered_set<StmtNum> lines = kv.second;
            std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
                cfgStorage->getGraph(proc);
            for (StmtNum line : lines) {
                std::unordered_set<StmtNum> children = graph[line][AppConstants::CHILDREN];
                for (StmtNum child : children) {
                    if (stmttypeLines2.find(child) != stmttypeLines2.end()) {
                        res.push_back({std::to_string(line), std::to_string(child)});
                    }
                }
            }
        }
    }
    else {
        std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines = getProcedureLines(stmttypeLines2);

        for (auto kv : procedure_lines) {
            ProcName proc = kv.first;
            std::unordered_set<StmtNum> lines = kv.second;
            std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
                cfgStorage->getGraph(proc);
            for (StmtNum line : lines) {
                std::unordered_set<StmtNum> parents = graph[line][AppConstants::PARENTS];
                for (StmtNum p : parents) {
                    if (stmttypeLines1.find(p) != stmttypeLines1.end()) {
                        res.push_back({std::to_string(p), std::to_string(line)});
                    }
                }
            }
        }
    }
    return res;
}

std::vector<std::vector<std::string>> NextHandler::handleWildcardWildcard() {
    std::unordered_set<ProcName> procedures = procStorage->getProcNames();
    std::vector<std::vector<std::string>> res;

    for (ProcName proc : procedures) {
        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
            cfgStorage->getGraph(proc);

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

// returns {lineNum, lineNum}
std::vector<std::vector<std::string>> NextHandler::handleIntIntTransitive(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    StmtNum n1 = stoi(paramString1);
    StmtNum n2 = stoi(paramString2);
    ProcName proc1 = procStorage->getProcedure(n1);
    ProcName proc2 = procStorage->getProcedure(n1);
    std::vector<std::vector<std::string>> res;

    if (proc1 == AppConstants::PROCEDURE_DOES_NOT_EXIST || proc2 == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }
    else if (proc1 != proc2) {
        return res;
    }

    std::deque<std::vector<StmtNum>> queue;
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
        cfgStorage->getGraph(proc1);
    for (StmtNum child : graph[n1][AppConstants::CHILDREN]) {
        if (child == n2) {
            res.push_back({paramString1, paramString2});
            return res;
        }
        queue.push_back({n1, child});
    }

    std::unordered_set<StmtNum> seen;
    while (!queue.empty()) {
        std::vector<StmtNum> curr = queue.front();
        queue.pop_front();

        if (seen.find(curr[1]) != seen.end()) {
            continue;
        }
        seen.insert(curr[1]);

        if (curr[1] == n2) {
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
std::vector<std::vector<std::string>> NextHandler::handleIntWildcardTransitive(Parameter param1) {
    std::string paramString1 = param1.getValue();
    StmtNum n1 = stoi(paramString1);
    ProcName proc1 = procStorage->getProcedure(n1);
    std::vector<std::vector<std::string>> res;
    if (proc1 == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }

    std::deque<std::vector<StmtNum>> queue;
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
        cfgStorage->getGraph(proc1);
    for (StmtNum child : graph[n1][AppConstants::CHILDREN]) {
        queue.push_back({n1, child});
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
std::vector<std::vector<std::string>> NextHandler::handleWildcardIntTransitive(Parameter param2) {
    std::string paramString2 = param2.getValue();
    StmtNum n2 = stoi(paramString2);
    ProcName proc = procStorage->getProcedure(n2);
    std::vector<std::vector<std::string>> res;
    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }

    std::deque<std::vector<StmtNum>> queue;
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
        cfgStorage->getGraph(proc);
    for (StmtNum parent : graph[n2][AppConstants::PARENTS]) {
        queue.push_back({parent, n2});
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

        for (StmtNum parent : graph[curr[0]][AppConstants::PARENTS]) {
            queue.push_back({parent, curr[1]});
        }
    }
    return res;
}

// returns {param1Num, lineNum}
std::vector<std::vector<std::string>> NextHandler::handleStmttypeIntTransitive(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    StmtNum n2 = stoi(paramString2);
    Stmt type = param1.getTypeString();
    ProcName proc = procStorage->getProcedure(n2);
    std::vector<std::vector<std::string>> res;

    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::deque<std::vector<StmtNum>> queue;
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
        cfgStorage->getGraph(proc);
    for (StmtNum parent : graph[n2][AppConstants::PARENTS]) {
        queue.push_back({parent, n2});
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

        for (StmtNum parent : graph[curr[0]][AppConstants::PARENTS]) {
            queue.push_back({parent, curr[1]});
        }
    }
    return res;
}

// returns {lineNum, param2Num}
std::vector<std::vector<std::string>> NextHandler::handleIntStmttypeTransitive(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    StmtNum n1 = stoi(paramString1);
    Stmt type = param2.getTypeString();
    ProcName proc = procStorage->getProcedure(n1);
    std::vector<std::vector<std::string>> res;

    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::deque<std::vector<StmtNum>> queue;
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
        cfgStorage->getGraph(proc);
    for (StmtNum child : graph[n1][AppConstants::CHILDREN]) {
        queue.push_back({n1, child});
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
std::vector<std::vector<std::string>> NextHandler::handleStmttypeWildcardTransitive(Parameter param1) {
    std::string paramString1 = param1.getValue();
    Stmt type = param1.getTypeString();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines = getProcedureLines(stmttypeLines);

    for (auto kv : procedure_lines) {
        ProcName proc = kv.first;
        std::unordered_set<StmtNum> lines = kv.second;

        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
            cfgStorage->getGraph(proc);
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
std::vector<std::vector<std::string>> NextHandler::handleWildcardStmttypeTransitive(Parameter param2) {
    std::string paramString1 = param2.getValue();
    Stmt type = param2.getTypeString();
    std::vector<std::vector<std::string>> res;

    std::unordered_set<StmtNum> stmttypeLines = stmtStorage->getStatementNumbers(type);
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines = getProcedureLines(stmttypeLines);

    for (auto kv : procedure_lines) {
        ProcName proc = kv.first;
        std::unordered_set<StmtNum> lines = kv.second;

        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
            cfgStorage->getGraph(proc);
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

                for (StmtNum parent : graph[curr[0]][AppConstants::PARENTS]) {
                    queue.push_back({parent, curr[1]});
                }
            }
        }
    }
    return res;
}

std::vector<std::vector<std::string>> NextHandler::handleStmttypeStmttypeTransitive(Parameter param1,
                                                                                    Parameter param2) {
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

        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
            cfgStorage->getGraph(proc);
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

std::vector<std::vector<std::string>> NextHandler::handleWildcardWildcardTransitive() {
    std::unordered_set<ProcName> procedures = procStorage->getProcNames();
    std::vector<std::vector<std::string>> res;

    for (ProcName proc : procedures) {
        std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
            cfgStorage->getGraph(proc);

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
std::unordered_map<ProcName, std::unordered_set<StmtNum>>
NextHandler::getProcedureLines(std::unordered_set<StmtNum> statementNumbers) {
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines;
    for (StmtNum num : statementNumbers) {
        ProcName proc = procStorage->getProcedure(num);
        procedure_lines[proc].insert(num);
    }
    return procedure_lines;
};

std::vector<std::vector<std::string>> NextHandler::handleNonTransitive(Parameter param1, Parameter param2,
                                                                       bool isFixedIntParam1, bool isFixedIntParam2,
                                                                       bool isWildCardParam1, bool isWildCardParam2,
                                                                       bool isTypedStmtParam1, bool isTypedStmtParam2) {

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

std::vector<std::vector<std::string>> NextHandler::handleTransitive(Parameter param1, Parameter param2,
                                                                    bool isFixedIntParam1, bool isFixedIntParam2,
                                                                    bool isWildCardParam1, bool isWildCardParam2,
                                                                    bool isTypedStmtParam1, bool isTypedStmtParam2) {

    if (isFixedIntParam1) {
        if (isFixedIntParam2) {
            return handleIntIntTransitive(param1, param2);
        }
        else if (isTypedStmtParam2) {
            return handleIntStmttypeTransitive(param1, param2);
        }
        else if (isWildCardParam2) {
            return handleIntWildcardTransitive(param1);
        }
    }
    else if (isTypedStmtParam1) {
        if (isFixedIntParam2) {
            return handleStmttypeIntTransitive(param1, param2);
        }
        else if (isTypedStmtParam2) {
            return handleStmttypeStmttypeTransitive(param1, param2);
        }
        else if (isWildCardParam2) {
            return handleStmttypeWildcardTransitive(param1);
        }
    }
    else if (isWildCardParam1) {
        if (isFixedIntParam2) {
            return handleWildcardIntTransitive(param2);
        }
        else if (isTypedStmtParam2) {
            return handleWildcardStmttypeTransitive(param2);
        }
        else if (isWildCardParam2) {
            return handleWildcardWildcardTransitive();
        }
    }
    return std::vector<std::vector<std::string>>();
}
