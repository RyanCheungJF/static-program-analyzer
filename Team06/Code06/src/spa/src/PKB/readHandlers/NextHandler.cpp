#include "NextHandler.h"

NextHandler::NextHandler(std::shared_ptr<CFGStorage> cfgStorage, std::shared_ptr<StmtStorage> stmtStorage,
                         std::shared_ptr<ProcedureStorage> procStorage) {
    this->cfgStorage = cfgStorage;
    this->stmtStorage = stmtStorage;
    this->procStorage = procStorage;
}

std::vector<std::vector<std::string>> NextHandler::handle(Parameter param1, Parameter param2) {
    bool isFixedIntParam1 = param1.isFixedInt();
    bool isFixedIntParam2 = param2.isFixedInt();
    bool isWildCardParam1 = param1.isWildcard();
    bool isWildCardParam2 = param2.isWildcard();
    bool isTypedStmtParam1 = Parameter::isStatementRef(param1) && !isFixedIntParam1 && !isWildCardParam1;
    bool isTypedStmtParam2 = Parameter::isStatementRef(param2) && !isFixedIntParam2 && !isWildCardParam2;

    if (this->isTransitive) {
        return handleTransitive(param1, param2, isFixedIntParam1, isFixedIntParam2, isWildCardParam1, isWildCardParam2,
                                isTypedStmtParam1, isTypedStmtParam2);
    }
    return handleNonTransitive(param1, param2, isFixedIntParam1, isFixedIntParam2, isWildCardParam1, isWildCardParam2,
                               isTypedStmtParam1, isTypedStmtParam2);
}

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

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>>& graph =
        cfgStorage->getGraph(proc1);
    if (graph[n1][AppConstants::CHILDREN].find(n2) != graph[n1][AppConstants::CHILDREN].end()) {
        res.push_back({paramString1, paramString2});
    }
    return res;
}

std::vector<std::vector<std::string>> NextHandler::oneIntOneWildcardNonT(Parameter intParam, bool isFindChildren) {
    StmtNum intValue = intParam.getIntValue();
    ProcName proc = procStorage->getProcedure(intValue);

    std::vector<std::vector<std::string>> res;

    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }

    std::unordered_set<StmtNum> emptyFilter;
    addCFGRelatives(res, proc, intValue, isFindChildren, emptyFilter, AppConstants::IS_EARLY_RETURN);

    return res;
}

std::vector<std::vector<std::string>> NextHandler::oneIntOneStmtNonT(Parameter intParam, Parameter stmtParam,
                                                                     bool isFindChildren) {
    StmtNum intValue = intParam.getIntValue();
    Stmt stmtType = stmtParam.getTypeString();

    ProcName proc = procStorage->getProcedure(intValue);
    std::vector<std::vector<std::string>> res;

    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }

    addCFGRelatives(res, proc, intValue, isFindChildren, stmtStorage->getStatementNumbers(stmtType),
                    !AppConstants::IS_EARLY_RETURN);
    return res;
}

std::vector<std::vector<std::string>> NextHandler::oneStmtOneWildcardNonT(Parameter stmtParam, bool isFindChildren) {
    Stmt stmtType = stmtParam.getTypeString();

    std::vector<std::vector<std::string>> res;
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines =
        getProcedureLines(stmtStorage->getStatementNumbers(stmtType));

    std::unordered_set<StmtNum> emptyFilter;
    for (const auto& kv : procedure_lines) {
        ProcName proc = kv.first;
        const std::unordered_set<StmtNum>& lines = kv.second;
        for (StmtNum line : lines) {
            addCFGRelatives(res, proc, line, isFindChildren, emptyFilter, !AppConstants::IS_EARLY_RETURN);
        }
    }
    return res;
}

std::vector<std::vector<std::string>> NextHandler::handleStmttypeStmttype(Parameter param1, Parameter param2) {
    Stmt type1 = param1.getTypeString();
    Stmt type2 = param2.getTypeString();
    std::vector<std::vector<std::string>> res;
    if (param1 == param2) {
        return res;
    }

    bool isLines1Smaller =
        stmtStorage->getStatementNumbers(type1).size() < stmtStorage->getStatementNumbers(type2).size();
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines;

    if (isLines1Smaller) {
        procedure_lines = getProcedureLines(stmtStorage->getStatementNumbers(type1));
    }
    else {
        procedure_lines = getProcedureLines(stmtStorage->getStatementNumbers(type2));
    }

    for (const auto& kv : procedure_lines) {
        ProcName proc = kv.first;
        const std::unordered_set<StmtNum>& lines = kv.second;
        for (StmtNum line : lines) {
            if (isLines1Smaller) {
                addCFGRelatives(res, proc, line, IS_FIND_CHILDREN, stmtStorage->getStatementNumbers(type2),
                                !AppConstants::IS_EARLY_RETURN);
            }
            else {
                addCFGRelatives(res, proc, line, !IS_FIND_CHILDREN, stmtStorage->getStatementNumbers(type1),
                                !AppConstants::IS_EARLY_RETURN);
            }
        }
    }

    return res;
}

std::vector<std::vector<std::string>> NextHandler::handleWildcardWildcard() {
    std::vector<std::vector<std::string>> res;

    for (ProcName proc : procStorage->getProcNames()) {
        const auto& graph = cfgStorage->getGraph(proc);

        for (const auto& kv : graph) {
            StmtNum p = kv.first;

            const auto& children = findGraphRelative(graph, p, AppConstants::CHILDREN);
            for (StmtNum child : children) {
                std::vector<std::string> val = {std::to_string(p), std::to_string(child)};
                res.push_back(val);

                // early return if at least 1 value in results since
                // we don't need all values for wildcard-wildcard params
                return res;
            }
        }
    }
    return res;
}

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
    const auto& graph = cfgStorage->getGraph(proc1);

    const auto& stmt1Children = findGraphRelative(graph, n1, AppConstants::CHILDREN);

    for (StmtNum child : stmt1Children) {
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

        // check with darren
        if (graph.find(curr[1]) == graph.end()) {
            continue;
        }

        const auto& children = findGraphRelative(graph, curr[1], AppConstants::CHILDREN);

        for (StmtNum child : children) {
            queue.push_back({curr[0], child});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> NextHandler::oneIntOneWildcardTransitive(Parameter intParam,
                                                                               bool isFindChildren) {
    StmtNum intValue = intParam.getIntValue();

    ProcName proc = procStorage->getProcedure(intValue);
    std::vector<std::vector<std::string>> res;
    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }

    std::deque<std::vector<StmtNum>> queue;
    const auto& graph = cfgStorage->getGraph(proc);
    initializeQueue(queue, graph, intValue, isFindChildren);

    std::unordered_set<StmtNum> emptyFilter;
    addCFGRelativesTransitive(res, graph, queue, isFindChildren, emptyFilter, AppConstants::IS_EARLY_RETURN);

    return res;
}

std::vector<std::vector<std::string>> NextHandler::oneIntOneStmtTransitive(Parameter intParam, Parameter stmtParam,
                                                                           bool isFindChildren) {
    StmtNum intValue = intParam.getIntValue();
    Stmt stmtType = stmtParam.getTypeString();

    ProcName proc = procStorage->getProcedure(intValue);
    std::vector<std::vector<std::string>> res;

    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }

    std::deque<std::vector<StmtNum>> queue;
    const auto& graph = cfgStorage->getGraph(proc);
    initializeQueue(queue, graph, intValue, isFindChildren);

    addCFGRelativesTransitive(res, graph, queue, isFindChildren, stmtStorage->getStatementNumbers(stmtType),
                              !AppConstants::IS_EARLY_RETURN);

    return res;
}

std::vector<std::vector<std::string>> NextHandler::oneStmtOneWildcardTransitive(Parameter stmtParam,
                                                                                bool isFindChildren) {
    Stmt stmtType = stmtParam.getTypeString();

    std::vector<std::vector<std::string>> res;

    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines =
        getProcedureLines(stmtStorage->getStatementNumbers(stmtType));

    for (const auto& kv : procedure_lines) {
        ProcName proc = kv.first;
        const std::unordered_set<StmtNum>& lines = kv.second;

        const auto& graph = cfgStorage->getGraph(proc);
        for (StmtNum line : lines) {
            std::deque<std::vector<StmtNum>> queue;
            initializeQueue(queue, graph, line, isFindChildren);

            std::unordered_set<StmtNum> emptyFilter;
            addCFGRelativesTransitive(res, graph, queue, isFindChildren, emptyFilter, !AppConstants::IS_EARLY_RETURN);
        }
    }
    return res;
}

std::vector<std::vector<std::string>> NextHandler::handleStmttypeStmttypeTransitive(Parameter param1,
                                                                                    Parameter param2) {
    Stmt type = param1.getTypeString();
    Stmt type2 = param2.getTypeString();

    std::vector<std::vector<std::string>> res;

    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines =
        getProcedureLines(stmtStorage->getStatementNumbers(type));

    for (const auto& kv : procedure_lines) {
        ProcName proc = kv.first;
        const std::unordered_set<StmtNum>& lines = kv.second;

        const auto& graph = cfgStorage->getGraph(proc);
        for (StmtNum line : lines) {

            std::deque<std::vector<StmtNum>> queue;
            initializeQueue(queue, graph, line, IS_FIND_CHILDREN);

            std::unordered_set<StmtNum>& type2StmtNums = stmtStorage->getStatementNumbers(type2);
            addCFGRelativesTransitive(res, graph, queue, IS_FIND_CHILDREN, type2StmtNums,
                                      !AppConstants::IS_EARLY_RETURN);
        }
    }

    // if both synonyms are the same, filter non-matching answers
    if (param1 == param2) {
        res.erase(std::remove_if(res.begin(), res.end(),
                                 [&](const std::vector<std::string>& pair) {
                                     return pair[0] != pair[1];
                                 }),
                  res.end());
    }
    return res;
}

// helper functions
std::unordered_map<ProcName, std::unordered_set<StmtNum>>
NextHandler::getProcedureLines(std::unordered_set<StmtNum>& statementNumbers) {
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> procedure_lines;
    for (StmtNum num : statementNumbers) {
        ProcName proc = procStorage->getProcedure(num);
        procedure_lines[proc].insert(num);
    }
    return procedure_lines;
};

void NextHandler::addCFGRelatives(std::vector<std::vector<std::string>>& res, ProcName proc, StmtNum num,
                                  bool isFindChildren, std::unordered_set<StmtNum>& filterSet, bool isEarlyReturn) {

    const auto& graph = cfgStorage->getGraph(proc);

    const std::unordered_set<StmtNum>& relatives =
        findGraphRelative(graph, num, isFindChildren ? AppConstants::CHILDREN : AppConstants::PARENTS);

    for (StmtNum relative : relatives) {
        if (!filterSet.empty()) {
            if (filterSet.find(relative) == filterSet.end()) {
                continue;
            }
        }

        if (isFindChildren) {
            res.push_back({std::to_string(num), std::to_string(relative)});
        }
        else {
            res.push_back({std::to_string(relative), std::to_string(num)});
        }

        if (isEarlyReturn) {
            return;
        }
    }
}

void NextHandler::initializeQueue(std::deque<std::vector<StmtNum>>& queue, const CFG& graph, StmtNum num,
                                  bool isFindChildren) {

    const auto& relatives =
        findGraphRelative(graph, num, isFindChildren ? AppConstants::CHILDREN : AppConstants::PARENTS);

    for (StmtNum relative : relatives) {

        Stmt type1 = isFindChildren ? stmtStorage->getStatementType(num) : stmtStorage->getStatementType(relative);
        Stmt type2 = isFindChildren ? stmtStorage->getStatementType(relative) : stmtStorage->getStatementType(num);

        if (isFindChildren) {
            queue.push_back({num, relative});
        }
        else {
            queue.push_back({relative, num});
        }
    }
}

void NextHandler::addCFGRelativesTransitive(std::vector<std::vector<std::string>>& res, const CFG& graph,
                                            std::deque<std::vector<StmtNum>>& queue, bool isFindChildren,
                                            std::unordered_set<StmtNum>& filterSet, bool isEarlyReturn) {
    std::unordered_set<StmtNum> seen;
    int pos = isFindChildren ? 1 : 0;

    while (!queue.empty()) {
        std::vector<StmtNum> curr = queue.front();
        queue.pop_front();

        if (seen.find(curr[pos]) != seen.end()) {
            continue;
        }
        seen.insert(curr[pos]);
        if (!filterSet.empty()) {
            if (filterSet.find(curr[pos]) != filterSet.end()) {
                res.push_back({std::to_string(curr[0]), std::to_string(curr[1])});
            }
        }
        else {
            res.push_back({std::to_string(curr[0]), std::to_string(curr[1])});
        }

        if (isEarlyReturn) {
            return;
        }

        if (graph.find(curr[pos]) == graph.end()) {
            continue;
        }

        const auto& relatives =
            findGraphRelative(graph, curr[pos], isFindChildren ? AppConstants::CHILDREN : AppConstants::PARENTS);

        for (StmtNum relative : relatives) {
            if (isFindChildren) {
                queue.push_back({curr[0], relative});
            }
            else {
                queue.push_back({relative, curr[1]});
            }
        }
    }
}

const std::unordered_set<StmtNum>& NextHandler::findGraphRelative(const CFG& graph, StmtNum num,
                                                                  std::string relativeType) {
    if (graph.find(num) == graph.end()) {
        return emptySet;
    }

    const auto& allRelatives = graph.at(num);
    if (allRelatives.find(relativeType) == allRelatives.end()) {
        return emptySet;
    }

    return allRelatives.at(relativeType);
}

std::vector<std::vector<std::string>> NextHandler::handleNonTransitive(Parameter param1, Parameter param2,
                                                                       bool isFixedIntParam1, bool isFixedIntParam2,
                                                                       bool isWildCardParam1, bool isWildCardParam2,
                                                                       bool isTypedStmtParam1, bool isTypedStmtParam2) {

    if (isFixedIntParam1) {
        if (isFixedIntParam2) {
            return handleIntInt(param1, param2);
        }
        else if (isTypedStmtParam2) {
            return oneIntOneStmtNonT(param1, param2, IS_FIND_CHILDREN);
        }
        else if (isWildCardParam2) {
            return oneIntOneWildcardNonT(param1, IS_FIND_CHILDREN);
        }
    }
    else if (isTypedStmtParam1) {
        if (isFixedIntParam2) {
            return oneIntOneStmtNonT(param2, param1, !IS_FIND_CHILDREN);
        }
        else if (isTypedStmtParam2) {
            return handleStmttypeStmttype(param1, param2);
        }
        else if (isWildCardParam2) {
            return oneStmtOneWildcardNonT(param1, IS_FIND_CHILDREN);
        }
    }
    else if (isWildCardParam1) {
        if (isFixedIntParam2) {
            return oneIntOneWildcardNonT(param2, !IS_FIND_CHILDREN);
        }
        else if (isTypedStmtParam2) {
            return oneStmtOneWildcardNonT(param2, !IS_FIND_CHILDREN);
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
            return oneIntOneStmtTransitive(param1, param2, IS_FIND_CHILDREN);
        }
        else if (isWildCardParam2) {
            return oneIntOneWildcardTransitive(param1, IS_FIND_CHILDREN);
        }
    }
    else if (isTypedStmtParam1) {
        if (isFixedIntParam2) {
            return oneIntOneStmtTransitive(param2, param1, !IS_FIND_CHILDREN);
        }
        else if (isTypedStmtParam2) {
            return handleStmttypeStmttypeTransitive(param1, param2);
        }
        else if (isWildCardParam2) {
            return oneStmtOneWildcardTransitive(param1, IS_FIND_CHILDREN);
        }
    }
    else if (isWildCardParam1) {
        if (isFixedIntParam2) {
            return oneIntOneWildcardTransitive(param2, !IS_FIND_CHILDREN);
        }
        else if (isTypedStmtParam2) {
            return oneStmtOneWildcardTransitive(param2, !IS_FIND_CHILDREN);
        }
        else if (isWildCardParam2) {
            return handleWildcardWildcard();
        }
    }
    return std::vector<std::vector<std::string>>();
}
