#include "AffectsHandler.h"

AffectsHandler::AffectsHandler(std::shared_ptr<CFGStorage> cfgStorage, std::shared_ptr<StmtStorage> stmtStorage,
                               std::shared_ptr<ProcedureStorage> procStorage,
                               std::shared_ptr<ModifiesUsesStorage> modifiesStorage,
                               std::shared_ptr<ModifiesUsesStorage> usesStorage, bool isTransitive) {
    this->cfgStorage = cfgStorage;
    this->stmtStorage = stmtStorage;
    this->procStorage = procStorage;
    this->modifiesStorage = modifiesStorage;
    this->usesStorage = usesStorage;
    this->isTransitive = isTransitive;
}

std::vector<std::vector<std::string>> AffectsHandler::handle(Parameter param1, Parameter param2) {

    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    ParameterType paramType1 = param1.getType();
    ParameterType paramType2 = param2.getType();

    bool isFixedIntParam1 = paramType1 == ParameterType::FIXED_INT;
    bool isFixedIntParam2 = paramType2 == ParameterType::FIXED_INT;
    bool isWildCardParam1 = paramType1 == ParameterType::STMT || paramType1 == ParameterType::WILDCARD ||
                            paramType1 == ParameterType::ASSIGN;
    bool isWildCardParam2 = paramType2 == ParameterType::STMT || paramType2 == ParameterType::WILDCARD ||
                            paramType2 == ParameterType::ASSIGN;

    std::vector<std::vector<std::string>> temp;
    if (isTransitive) {
        temp = handleTransitive(param1.getValue(), param2.getValue(), isFixedIntParam1, isFixedIntParam2,
                                isWildCardParam1, isWildCardParam2);
    }
    else {
        temp = handleNonTransitive(param1.getValue(), param2.getValue(), isFixedIntParam1, isFixedIntParam2,
                                   isWildCardParam1, isWildCardParam2);
    }

    if ((paramString1 == paramString2) && (paramType1 != ParameterType::WILDCARD)) {
        std::vector<std::vector<std::string>> res;
        for (std::vector<std::string> curr : temp) {
            if (curr[0] == curr[1]) {
                res.push_back(curr);
            }
        }
        return res;
    }
    else {
        return temp;
    }
}

std::vector<std::vector<std::string>> AffectsHandler::handleIntInt(StmtNum a1, StmtNum a2) {
    std::string paramString1 = std::to_string(a1);
    std::string paramString2 = std::to_string(a2);
    ProcName proc1 = procStorage->getProcedure(a1);
    ProcName proc2 = procStorage->getProcedure(a2);

    // either not in any procedure, or both are not in the same procedure
    if (proc1 == AppConstants::PROCEDURE_DOES_NOT_EXIST || proc2 == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return {};
    }
    else if (proc1 != proc2) {
        return {};
    }

    // if both are not assign statements, should also just return nothing already
    std::unordered_set<StmtNum> statements = *procStorage->getProcedureStatementNumbers(proc1);
    std::unordered_set<StmtNum> assignStatements = getAssignStatements(statements);
    if (assignStatements.find(a1) == assignStatements.end() || assignStatements.find(a2) == assignStatements.end()) {
        return {};
    }

    std::unordered_set<Ent> variablesModifiedInA1 = *modifiesStorage->getRightItems(a1);
    std::unordered_set<Ent> variablesUsedInA2 = *usesStorage->getRightItems(a2);
    std::unordered_set<Ent> commonVariables = getCommonVariables(variablesModifiedInA1, variablesUsedInA2);
    if (commonVariables.empty()) {
        return {};
    }

    bool canReach = checkCanReach(a1, a2, proc1, commonVariables);
    if (!canReach) {
        return {};
    }

    return {{paramString1, paramString2}};
    //    std::vector<std::vector<std::string>> res;
    //    res.push_back({paramString1, paramString2});
    //    return res;
}

std::vector<std::vector<std::string>> AffectsHandler::handleWildcardInt(StmtNum a2) {
    return nonTransitiveOneIntOneWildcard(AppConstants::NOT_USED_FIELD, a2);
}

std::vector<std::vector<std::string>> AffectsHandler::handleIntWildcard(StmtNum a1) {
    return nonTransitiveOneIntOneWildcard(a1, AppConstants::NOT_USED_FIELD);
}

std::vector<std::vector<std::string>> AffectsHandler::handleWildcardWildcard(ProcName proc) {

    std::vector<std::vector<std::string>> res;
    std::unordered_set<ProcName> allProcedures;

    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        allProcedures = *procStorage->getProcNames();
    } else {
        allProcedures = {proc};
    }

    for (ProcName proc : allProcedures) {
        std::unordered_set<StmtNum> procStatements = *procStorage->getProcedureStatementNumbers(proc);
        std::unordered_set<StmtNum> assignStatements = getAssignStatements(procStatements);

        for (StmtNum a1 : assignStatements) {
            std::vector<std::vector<std::string>> temp = handleIntWildcard(a1);
            for (std::vector<std::string> val : temp) {
                res.push_back(val);
            }
        }
    }
    return res;
}

// Affects*
std::vector<std::vector<std::string>> AffectsHandler::handleIntIntTransitive(StmtNum a1, StmtNum a2) {
    ProcName proc1 = procStorage->getProcedure(a1);
    ProcName proc2 = procStorage->getProcedure(a2);
    std::vector<std::vector<std::string>> res;

    if (proc1 == AppConstants::PROCEDURE_DOES_NOT_EXIST || proc2 == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }
    else if (proc1 != proc2) {
        return res;
    }

    std::unordered_map<StmtNum, unordered_set<StmtNum>> hashmap = buildAffectsGraph(false, proc1);
    std::unordered_set<std::pair<StmtNum, StmtNum>, hashFunctionAffectsT> seen;
    std::deque<std::pair<StmtNum, StmtNum>> queue;
    for (StmtNum num : hashmap[a1]) {
        queue.push_back({a1, num});
    }

    // hop until we reach a2. else loop terminates once it has seen all pairs (prevent infinite loop)
    while (!queue.empty()) {
        std::pair<StmtNum, StmtNum> curr = queue.front();
        queue.pop_front();

        if (curr.second == a2) {
            res.push_back({std::to_string(a1), std::to_string(a2)});
            return res;
        }

        if (seen.find(curr) != seen.end()) {
            continue;
        }
        seen.insert(curr);

        for (StmtNum num : hashmap[curr.second]) {
            queue.push_front({curr.second, num});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> AffectsHandler::handleIntWildcardTransitive(StmtNum a1) {
    ProcName proc1 = procStorage->getProcedure(a1);
    if (proc1 == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return {};
    }
    return bfsTraversalOneWildcard(a1, AppConstants::NOT_USED_FIELD);
}

std::vector<std::vector<std::string>> AffectsHandler::handleWildcardIntTransitive(StmtNum a2) {
    ProcName proc2 = procStorage->getProcedure(a2);
    if (proc2 == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return {};
    }

    return bfsTraversalOneWildcard(AppConstants::NOT_USED_FIELD, a2);
}

std::vector<std::vector<std::string>> AffectsHandler::handleWildcardWildcardTransitive() {
    std::vector<std::vector<std::string>> res;
    std::unordered_map<StmtNum, unordered_set<StmtNum>> hashmap = buildAffectsGraph(false, AppConstants::PROCEDURE_DOES_NOT_EXIST);

    std::unordered_set<std::tuple<StmtNum, StmtNum, StmtNum>, hashFunctionTuple> seen;
    std::deque<std::tuple<StmtNum, StmtNum, StmtNum>> queue;

    for (auto kv : hashmap) {
        StmtNum a1 = kv.first;
        std::unordered_set<StmtNum> nums = kv.second;
        for (StmtNum num : nums) {
            queue.push_back({a1, a1, num});
        }
    }

    while (!queue.empty()) {
        std::tuple<StmtNum, StmtNum, StmtNum> curr = queue.front();
        queue.pop_front();
        if (seen.find(curr) != seen.end()) {
            continue;
        }
        seen.insert(curr);

        for (StmtNum num : hashmap[get<2>(curr)]) {
            queue.push_back({get<0>(curr), get<2>(curr), num});
        }
    }

    // remove duplicates
    std::unordered_set<std::pair<StmtNum, StmtNum>, hashFunctionAffectsT> temp;
    for (std::tuple<StmtNum, StmtNum, StmtNum> curr : seen) {
        temp.insert({get<0>(curr), get<2>(curr)});
    }

    for (std::pair<StmtNum, StmtNum> p : temp) {
        res.push_back({std::to_string(p.first), std::to_string(p.second)});
    }

    return res;
}

// helper functions
//TODO: optimise this to just return Ent instead of the entire data structure
std::unordered_set<Ent> AffectsHandler::getCommonVariables(std::unordered_set<Ent> variablesModifiedInA1,
                                                           std::unordered_set<Ent> variablesUsedInA2) {

    std::unordered_set<Ent> commonVariables;
    for (Ent e : variablesModifiedInA1) { // O(1) since there is really only 1 element
        if (variablesUsedInA2.find(e) != variablesUsedInA2.end()) {
            commonVariables.insert(e);
        }
    }

    return commonVariables;
}

std::vector<std::vector<std::string>>
AffectsHandler::handleNonTransitive(std::string param1value, std::string param2value, bool isFixedIntParam1,
                                    bool isFixedIntParam2, bool isWildCardParam1, bool isWildCardParam2) {
    if (isFixedIntParam1) {
        if (isFixedIntParam2) {
            return handleIntInt(stoi(param1value), stoi(param2value));
        }
        else if (isWildCardParam2) {
            return handleIntWildcard(stoi(param1value));
        }
    }
    else if (isWildCardParam1) {
        if (isFixedIntParam2) {
            return handleWildcardInt(stoi(param2value));
        }
        else if (isWildCardParam2) {
            return handleWildcardWildcard(AppConstants::PROCEDURE_DOES_NOT_EXIST);
        }
    }
    return std::vector<std::vector<std::string>>();
}

std::vector<std::vector<std::string>> AffectsHandler::handleTransitive(std::string param1value, std::string param2value,
                                                                       bool isFixedIntParam1, bool isFixedIntParam2,
                                                                       bool isWildCardParam1, bool isWildCardParam2) {
    if (isFixedIntParam1) {
        if (isFixedIntParam2) {
            return handleIntIntTransitive(stoi(param1value), stoi(param2value));
        }
        else if (isWildCardParam2) {
            return handleIntWildcardTransitive(stoi(param1value));
        }
    }
    else if (isWildCardParam1) {
        if (isFixedIntParam2) {
            return handleWildcardIntTransitive(stoi(param2value));
        }
        else if (isWildCardParam2) {
            return handleWildcardWildcardTransitive();
        }
    }
    return std::vector<std::vector<std::string>>();
}

std::unordered_map<StmtNum, unordered_set<StmtNum>> AffectsHandler::buildAffectsGraph(bool isInverted, ProcName proc) {

    // build the hop graph
    std::vector<std::vector<std::string>> allValidAffects = handleWildcardWildcard(proc);
    std::unordered_map<StmtNum, unordered_set<StmtNum>> hashmap;
    for (std::vector<std::string> p : allValidAffects) {
        isInverted ? hashmap[stoi(p[1])].insert(stoi(p[0])) : hashmap[stoi(p[0])].insert(stoi(p[1]));
    }
    return hashmap;
}

std::unordered_set<StmtNum> AffectsHandler::getAssignStatements(std::unordered_set<StmtNum> allProcStatements) {

    std::unordered_set<StmtNum> assignStatements;
    for (StmtNum num : allProcStatements) {
        std::unordered_set<Stmt> currStmtType = *stmtStorage->getStatementType(num);
        if (currStmtType.find(AppConstants::ASSIGN) != currStmtType.end()) {
            assignStatements.insert(num);
        }
    }
    return assignStatements;
}

std::vector<std::vector<std::string>> AffectsHandler::bfsTraversalOneWildcard(StmtNum a1, StmtNum a2) {
    bool isIntWildcard = a2 == AppConstants::NOT_USED_FIELD;
    ProcName proc = isIntWildcard ? procStorage->getProcedure(a1) : procStorage->getProcedure(a2);
    std::unordered_map<StmtNum, unordered_set<StmtNum>> hashmap = buildAffectsGraph(!isIntWildcard, proc);
    std::unordered_set<std::pair<StmtNum, StmtNum>, hashFunctionAffectsT> seen;
    std::deque<std::pair<StmtNum, StmtNum>> queue;

    // add from initial starting node
    for (StmtNum num : (isIntWildcard ? hashmap[a1] : hashmap[a2])) {
        isIntWildcard ? queue.push_back({a1, num}) : queue.push_back({num, a2});
    }

    // traverse until we see all possible combinations
    while (!queue.empty()) {
        std::pair<StmtNum, StmtNum> curr = queue.front();
        queue.pop_front();
        if (seen.find(curr) != seen.end()) {
            continue;
        }
        seen.insert(curr);

        unordered_set<StmtNum> nextNodes = (isIntWildcard ? hashmap[curr.second] : hashmap[curr.first]);
        for (StmtNum num : nextNodes) {
            if (isIntWildcard) {
                queue.push_back({curr.second, num});
            }
            else {
                queue.push_back({num, curr.first});
            }
        }
    }

    // remove potential duplicate entries
    std::unordered_set<std::pair<StmtNum, StmtNum>, hashFunctionAffectsT> temp;
    for (std::pair<StmtNum, StmtNum> p : seen) {
        isIntWildcard ? temp.insert({a1, p.second}) : temp.insert({p.first, a2});
    }

    std::vector<std::vector<std::string>> res;
    std::string paramString = (isIntWildcard ? std::to_string(a1) : std::to_string(a2));
    for (std::pair<StmtNum, StmtNum> p : temp) {
        isIntWildcard ? res.push_back({paramString, std::to_string(p.second)})
                      : res.push_back({std::to_string(p.first), paramString});
    }
    return res;
}

std::vector<std::vector<std::string>> AffectsHandler::nonTransitiveOneIntOneWildcard(StmtNum a1input, StmtNum a2input) {
    bool isIntWildcard = (a2input == AppConstants::NOT_USED_FIELD);
    std::string paramString = isIntWildcard ? std::to_string(a1input) : std::to_string(a2input);
    StmtNum currA = isIntWildcard ? a1input : a2input;
    ProcName proc = procStorage->getProcedure(currA);

    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return {};
    }

    std::unordered_set<StmtNum> procStatements = *procStorage->getProcedureStatementNumbers(proc);
    std::unordered_set<StmtNum> assignStatements = getAssignStatements(procStatements);

    if (assignStatements.find(currA) == assignStatements.end()) {
        return {};
    }

    std::unordered_set<Ent> variablesInCurrA =
        isIntWildcard ? *modifiesStorage->getRightItems(currA) : *usesStorage->getRightItems(currA);
    std::vector<std::vector<std::string>> res;

    for (StmtNum otherA : assignStatements) {

        std::unordered_set<Ent> variablesInOtherA =
            isIntWildcard ? *usesStorage->getRightItems(otherA) : *modifiesStorage->getRightItems(otherA);
        std::unordered_set<Ent> commonVariables = isIntWildcard
                                                      ? getCommonVariables(variablesInCurrA, variablesInOtherA)
                                                      : getCommonVariables(variablesInOtherA, variablesInCurrA);
        if (proc != procStorage->getProcedure(otherA)) {
            continue;
        }

        std::vector<std::vector<std::string>> temp =
            isIntWildcard ? handleIntInt(currA, otherA) : handleIntInt(otherA, currA);

        if (!temp.empty()) {
            isIntWildcard ? res.push_back({paramString, std::to_string(otherA)})
                          : res.push_back({std::to_string(otherA), paramString});
        }
    }

    return res;
}

bool AffectsHandler::checkModifiedAssignReadCall(std::unordered_set<Ent> commonVariables, StmtNum currentLine) {

    unordered_set<Ent> entitiesModifiedOnCurrentLine = *modifiesStorage->getRightItems(currentLine);

    // if a assignment, read, or procedure call, we check if the entitiesModifiedOnCurrentLine is the same as
    // commonVariables
    std::unordered_set<Stmt> currLineStmtType = *stmtStorage->getStatementType(currentLine);
    if (currLineStmtType.find(AppConstants::ASSIGN) != currLineStmtType.end() ||
        currLineStmtType.find(AppConstants::READ) != currLineStmtType.end() ||
        currLineStmtType.find(AppConstants::CALL) != currLineStmtType.end()) {

        for (Ent e : commonVariables) { // O(1) since there is only 1 variable
            if (entitiesModifiedOnCurrentLine.find(e) != entitiesModifiedOnCurrentLine.end()) {
                return true;
            }
        }
    }
    return false;
}

bool AffectsHandler::checkCanReach(StmtNum a1, StmtNum a2, ProcName proc, std::unordered_set<Ent> commonVariables) {

    // if either is not an assign statements, should also just return nothing already
    std::unordered_set<StmtNum> statements = *procStorage->getProcedureStatementNumbers(proc);
    std::unordered_set<StmtNum> assignStatements = getAssignStatements(statements);
    if (assignStatements.find(a1) == assignStatements.end() || assignStatements.find(a2) == assignStatements.end()) {
        return false;
    }

    if (commonVariables.size() == 0) {
        return false;
    }

    std::deque<std::pair<StmtNum, StmtNum>> queue;
    std::unordered_set<std::pair<StmtNum, StmtNum>, hashFunctionAffectsT> seen;
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
        cfgStorage->getGraph(proc);

    // curr.first is the previous node.
    // curr.second is the current node
    std::pair<StmtNum, StmtNum> p = {AppConstants::DUMMY_NODE, a1};
    queue.push_back(p);
    while (!queue.empty()) {
        std::pair<StmtNum, StmtNum> curr = queue.front();
        queue.pop_front();
        if (curr.second == a2 && curr.first != AppConstants::DUMMY_NODE) {
            return true;
        }
        else if (curr.first != AppConstants::DUMMY_NODE && checkModifiedAssignReadCall(commonVariables, curr.second)) {
            continue;
        }
        else if (seen.find(curr) != seen.end()) {
            continue;
        }

        seen.insert(curr);
        std::unordered_set<StmtNum> children = graph[curr.second][AppConstants::CHILDREN];
        for (StmtNum child : children) {
            queue.push_back({curr.second, child});
        }
    }
    return false;
}
