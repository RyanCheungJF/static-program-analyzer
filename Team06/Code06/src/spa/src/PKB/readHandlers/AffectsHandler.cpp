#include "AffectsHandler.h"

AffectsHandler::AffectsHandler(std::shared_ptr<CFGStorage>& cfgStorage, std::shared_ptr<StmtStorage>& stmtStorage,
                               std::shared_ptr<ProcedureStorage>& procStorage,
                               std::shared_ptr<ModifiesUsesStorage>& modifiesStorage,
                               std::shared_ptr<ModifiesUsesStorage>& usesStorage,
                               std::shared_ptr<ProcedureStorage>& procAssignStmtStorage) {
    this->cfgStorage = cfgStorage;
    this->stmtStorage = stmtStorage;
    this->procStorage = procStorage;
    this->procAssignStmtStorage = procAssignStmtStorage;
    this->modifiesStorage = modifiesStorage;
    this->usesStorage = usesStorage;
}

std::vector<std::vector<std::string>> AffectsHandler::handle(Parameter& param1, Parameter& param2) {

    StmtNum paramInt1 = param1.getIntValue();
    StmtNum paramInt2 = param2.getIntValue();

    bool isFixedIntParam1 = param1.isFixedInt();
    bool isFixedIntParam2 = param2.isFixedInt();
    bool isWildCardParam1 = param1.isWildcard();
    bool isWildCardParam2 = param2.isWildcard();
    bool isAssignStmtParam1 = param1.isAssign() || param1.isStmt();
    bool isAssignStmtParam2 = param2.isAssign() || param2.isStmt();

    std::vector<std::vector<std::string>> temp;
    if (this->isTransitive) {
        temp = handleTransitive(paramInt1, paramInt2, isFixedIntParam1, isFixedIntParam2, isWildCardParam1,
                                isWildCardParam2, isAssignStmtParam1, isAssignStmtParam2);
    }
    else {
        temp = handleNonTransitive(paramInt1, paramInt2, isFixedIntParam1, isFixedIntParam2, isWildCardParam1,
                                   isWildCardParam2, isAssignStmtParam1, isAssignStmtParam2);
    }

    if ((param1.getValue() == param2.getValue()) && !isWildCardParam1 && !isWildCardParam2) {
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

    std::vector<std::string> val = {paramString1, paramString2};
    if (intIntCache.find(val) != intIntCache.end() && intIntCache[val]) {
        return {val};
    }

    ProcName proc1 = procStorage->getProcedure(a1);
    ProcName proc2 = procStorage->getProcedure(a2);

    // either not in any procedure, or both are not in the same procedure
    if (proc1 == AppConstants::PROCEDURE_DOES_NOT_EXIST || proc2 == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        intIntCache[val] = false;
        return {};
    }
    else if (proc1 != proc2) {
        intIntCache[val] = false;
        return {};
    }

    // if both are not assign statements, should also just return nothing already
    if (!procAssignStmtStorage->checkProcedure(proc1, a1) || !procAssignStmtStorage->checkProcedure(proc1, a2)) {
        intIntCache[val] = false;
        return {};
    }

    std::unordered_set<Ent> variablesModifiedInA1 = modifiesStorage->getRightItems(a1);
    std::unordered_set<Ent> variablesUsedInA2 = usesStorage->getRightItems(a2);
    Ent commonVariable = getCommonVariable(variablesModifiedInA1, variablesUsedInA2);
    if (commonVariable == "") {
        intIntCache[val] = false;
        return {};
    }

    bool canReach = checkCanReach(a1, a2, proc1, commonVariable);
    if (!canReach) {
        intIntCache[val] = false;
        return {};
    }

    intIntCache[val] = true;
    std::vector<std::vector<std::string>> res = {val};
    return res;
}

std::vector<std::vector<std::string>> AffectsHandler::handleOneIntOneWildcard(StmtNum a1, StmtNum a2,
                                                                              bool isEarlyReturn) {

    bool isIntWildcard = a2 == AppConstants::NOT_USED_FIELD;

    if (isIntWildcard && intWildcardCache.find(a1) != intWildcardCache.end()) {
        return intWildcardCache[a1];
    }
    else if (wildcardIntCache.find(a2) != wildcardIntCache.end()) {
        return wildcardIntCache[a2];
    }

    std::vector<std::vector<std::string>> res =
        isIntWildcard ? nonTransitiveOneIntOneWildcard(a1, AppConstants::NOT_USED_FIELD, isEarlyReturn)
                      : nonTransitiveOneIntOneWildcard(AppConstants::NOT_USED_FIELD, a2, isEarlyReturn);

    if (isIntWildcard && !isEarlyReturn) {
        intWildcardCache[a1] = res;
    }
    else if (!isEarlyReturn) {
        wildcardIntCache[a2] = res;
    }
    return res;
}

std::vector<std::vector<std::string>> AffectsHandler::handleWildcardWildcard(ProcName proc, bool isEarlyReturn) {

    std::vector<std::vector<std::string>> res;
    std::unordered_set<ProcName> allProcedures;

    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        allProcedures = procStorage->getProcNames();
    }
    else {
        allProcedures = {proc};
    }

    for (ProcName proc : allProcedures) {
        const std::unordered_set<StmtNum>& assignStatements = procAssignStmtStorage->getProcedureStatementNumbers(proc);

        for (StmtNum a1 : assignStatements) {
            std::vector<std::vector<std::string>> temp =
                handleOneIntOneWildcard(a1, AppConstants::NOT_USED_FIELD, isEarlyReturn);

            for (std::vector<std::string>& val : temp) {
                res.push_back(val);

                if (isEarlyReturn) {
                    return res;
                }

                StmtNum a2 = std::stoi(val[1]);
                wildcardIntCache[a2].push_back(val);
            }
        }
    }

    return res;
}

// Affects*
std::vector<std::vector<std::string>> AffectsHandler::handleIntIntTransitive(StmtNum a1, StmtNum a2) {
    std::string paramString1 = std::to_string(a1);
    std::string paramString2 = std::to_string(a2);

    std::vector<std::string> val = {paramString1, paramString2};
    if (intIntTransitiveCache.find(val) != intIntTransitiveCache.end() && intIntTransitiveCache[val]) {
        return {val};
    }

    ProcName proc1 = procStorage->getProcedure(a1);
    ProcName proc2 = procStorage->getProcedure(a2);
    std::vector<std::vector<std::string>> res;

    if (proc1 == AppConstants::PROCEDURE_DOES_NOT_EXIST || proc2 == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        intIntTransitiveCache[val] = false;
        return res;
    }
    else if (proc1 != proc2) {
        intIntTransitiveCache[val] = false;
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
            std::vector<std::string> tempVal = {std::to_string(a1), std::to_string(a2)};
            intIntTransitiveCache[tempVal] = true;

            res.push_back(tempVal);
            return res;
        }

        if (seen.find(curr) != seen.end()) {
            continue;
        }
        seen.insert(curr);

        for (StmtNum num : hashmap[curr.second]) {
            queue.push_front({curr.second, num});

            std::vector<std::string> tempVal = {std::to_string(a1), std::to_string(num)};
            intIntTransitiveCache[tempVal] = true;
        }
    }

    std::vector<std::string> tempVal = {std::to_string(a1), std::to_string(a2)};
    intIntTransitiveCache[tempVal] = false;
    return res;
}

std::vector<std::vector<std::string>> AffectsHandler::handleOneIntOneSynTransitive(StmtNum a1, StmtNum a2) {

    bool isIntSyn = (a2 == AppConstants::NOT_USED_FIELD);

    ProcName proc = isIntSyn ? procStorage->getProcedure(a1) : procStorage->getProcedure(a2);
    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return {};
    }

    if (isIntSyn && intWildcardTransitiveCache.find(a1) != intWildcardTransitiveCache.end()) {
        return intWildcardTransitiveCache[a1];
    }
    else if (wildcardIntTransitiveCache.find(a2) != wildcardIntTransitiveCache.end()) {
        return wildcardIntTransitiveCache[a2];
    }

    isIntSyn ? intWildcardTransitiveCache[a1] = bfsTraversalOneWildcard(a1, AppConstants::NOT_USED_FIELD)
                  : wildcardIntTransitiveCache[a2] = bfsTraversalOneWildcard(AppConstants::NOT_USED_FIELD, a2);
    return isIntSyn ? intWildcardTransitiveCache[a1] : wildcardIntTransitiveCache[a2];
}

std::vector<std::vector<std::string>> AffectsHandler::handleSynSynTransitive() {

    std::vector<std::vector<std::string>> res;
    std::unordered_map<StmtNum, unordered_set<StmtNum>> hashmap =
        buildAffectsGraph(false, AppConstants::PROCEDURE_DOES_NOT_EXIST);

    // <starting node, current node, next node>
    std::unordered_set<std::tuple<StmtNum, StmtNum, StmtNum>, hashFunctionTuple> seen;
    std::deque<std::tuple<StmtNum, StmtNum, StmtNum>> queue;

    for (const auto& kv : hashmap) {
        StmtNum a1 = kv.first;
        const std::unordered_set<StmtNum>& nums = kv.second;
        for (StmtNum num : nums) {
            queue.push_back({a1, a1, num});

            std::vector<std::string> val = {std::to_string(a1), std::to_string(num)};
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
    for (const std::tuple<StmtNum, StmtNum, StmtNum>& curr : seen) {
        temp.insert({get<0>(curr), get<2>(curr)});
    }

    for (const std::pair<StmtNum, StmtNum>& p : temp) {
        std::vector<std::string> val = {std::to_string(p.first), std::to_string(p.second)};
        res.push_back(val);

        intWildcardTransitiveCache[p.first].push_back(val);
        wildcardIntTransitiveCache[p.second].push_back(val);
    }
    return res;
}

// helper functions
Ent AffectsHandler::getCommonVariable(std::unordered_set<Ent>& variablesModifiedInA1,
                                      std::unordered_set<Ent>& variablesUsedInA2) {

    for (Ent e : variablesModifiedInA1) { // O(1) since there is really only 1 element
        if (variablesUsedInA2.find(e) != variablesUsedInA2.end()) {
            return e;
        }
    }

    return "";
}

std::vector<std::vector<std::string>> AffectsHandler::handleNonTransitive(StmtNum param1Int, StmtNum param2Int,
                                                                          bool isFixedIntParam1, bool isFixedIntParam2,
                                                                          bool isWildCardParam1, bool isWildCardParam2,
                                                                          bool isAssignStmtParam1,
                                                                          bool isAssignStmtParam2) {
    if (isFixedIntParam1) {
        if (isFixedIntParam2) {
            return handleIntInt(param1Int, param2Int);
        }
        else if (isAssignStmtParam2) {
            return handleOneIntOneWildcard(param1Int, AppConstants::NOT_USED_FIELD, !AppConstants::IS_EARLY_RETURN);
        }
        else if (isWildCardParam2) {
            return handleOneIntOneWildcard(param1Int, AppConstants::NOT_USED_FIELD, AppConstants::IS_EARLY_RETURN);
        }
    }
    else if (isAssignStmtParam1) {
        if (isFixedIntParam2) {
            return handleOneIntOneWildcard(AppConstants::NOT_USED_FIELD, param2Int, !AppConstants::IS_EARLY_RETURN);
        }
        else if (isAssignStmtParam2 || isWildCardParam2) {
            return handleWildcardWildcard(AppConstants::PROCEDURE_DOES_NOT_EXIST, !AppConstants::IS_EARLY_RETURN);
        }
    }
    else if (isWildCardParam1) {
        if (isFixedIntParam2) {
            return handleOneIntOneWildcard(AppConstants::NOT_USED_FIELD, param2Int, AppConstants::IS_EARLY_RETURN);
        }
        else if (isAssignStmtParam2) {
            return handleWildcardWildcard(AppConstants::PROCEDURE_DOES_NOT_EXIST, !AppConstants::IS_EARLY_RETURN);
        }
        else if (isWildCardParam2) {
            return handleWildcardWildcard(AppConstants::PROCEDURE_DOES_NOT_EXIST, AppConstants::IS_EARLY_RETURN);
        }
    }
    return std::vector<std::vector<std::string>>();
}

std::vector<std::vector<std::string>> AffectsHandler::handleTransitive(StmtNum param1Int, StmtNum param2Int,
                                                                       bool isFixedIntParam1, bool isFixedIntParam2,
                                                                       bool isWildCardParam1, bool isWildCardParam2,
                                                                       bool isAssignStmtParam1,
                                                                       bool isAssignStmtParam2) {
    if (isFixedIntParam1) {
        if (isFixedIntParam2) {
            return handleIntIntTransitive(param1Int, param2Int);
        }
        else if (isAssignStmtParam2) {
            return handleOneIntOneSynTransitive(param1Int, AppConstants::NOT_USED_FIELD);
        }
        else if (isWildCardParam2) {
            return handleOneIntOneWildcard(param1Int, AppConstants::NOT_USED_FIELD, AppConstants::IS_EARLY_RETURN);
        }
    }
    else if (isAssignStmtParam1) {
        if (isFixedIntParam2) {
            return handleOneIntOneSynTransitive(AppConstants::NOT_USED_FIELD, param2Int);
        }
        else if (isAssignStmtParam2 || isWildCardParam2) {
            return handleSynSynTransitive();
        }
    }
    else if (isWildCardParam1) {
        if (isFixedIntParam2) {
            return handleOneIntOneWildcard(AppConstants::NOT_USED_FIELD, param2Int, AppConstants::IS_EARLY_RETURN);
        }
        else if (isAssignStmtParam2) {
            return handleSynSynTransitive();
        }
        else if (isWildCardParam2) {
            return handleWildcardWildcard(AppConstants::PROCEDURE_DOES_NOT_EXIST, AppConstants::IS_EARLY_RETURN);
        }
    }
    return std::vector<std::vector<std::string>>();
}

std::unordered_map<StmtNum, unordered_set<StmtNum>> AffectsHandler::buildAffectsGraph(bool isInverted, ProcName proc) {

    if (procAffectsGraphMap.find(proc) != procAffectsGraphMap.end() &&
        procAffectsGraphMap[proc].find(isInverted) != procAffectsGraphMap[proc].end()) {
        return procAffectsGraphMap[proc][isInverted];
    }

    // build the hop graph
    std::vector<std::vector<std::string>> allValidAffects =
        handleWildcardWildcard(proc, !AppConstants::IS_EARLY_RETURN);

    std::unordered_map<StmtNum, unordered_set<StmtNum>> hashmap;
    for (std::vector<std::string> p : allValidAffects) {
        isInverted ? hashmap[stoi(p[1])].insert(stoi(p[0])) : hashmap[stoi(p[0])].insert(stoi(p[1]));
    }

    procAffectsGraphMap[proc][isInverted] = hashmap;
    return procAffectsGraphMap[proc][isInverted];
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

        unordered_set<StmtNum>& nextNodes = (isIntWildcard ? hashmap[curr.second] : hashmap[curr.first]);
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
    for (const std::pair<StmtNum, StmtNum>& p : seen) {
        isIntWildcard ? temp.insert({a1, p.second}) : temp.insert({p.first, a2});
    }

    std::vector<std::vector<std::string>> res;
    std::string paramString = (isIntWildcard ? std::to_string(a1) : std::to_string(a2));
    for (const std::pair<StmtNum, StmtNum>& p : temp) {
        isIntWildcard ? res.push_back({paramString, std::to_string(p.second)})
                      : res.push_back({std::to_string(p.first), paramString});
    }

    for (std::vector<std::string> val : res) {
        intIntTransitiveCache[val] = true;
    }

    return res;
}

std::vector<std::vector<std::string>> AffectsHandler::nonTransitiveOneIntOneWildcard(StmtNum a1input, StmtNum a2input,
                                                                                     bool isEarlyReturn) {
    bool isIntWildcard = (a2input == AppConstants::NOT_USED_FIELD);
    std::string paramString = isIntWildcard ? std::to_string(a1input) : std::to_string(a2input);
    StmtNum currA = isIntWildcard ? a1input : a2input;
    ProcName proc = procStorage->getProcedure(currA);

    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return {};
    }

    const std::unordered_set<StmtNum>& assignStatements = procAssignStmtStorage->getProcedureStatementNumbers(proc);

    if (assignStatements.find(currA) == assignStatements.end()) {
        return {};
    }

    std::vector<std::vector<std::string>> res;
    for (StmtNum otherA : assignStatements) {

        if (proc != procStorage->getProcedure(otherA)) {
            continue;
        }

        std::vector<std::vector<std::string>> temp =
            isIntWildcard ? handleIntInt(currA, otherA) : handleIntInt(otherA, currA);

        if (!temp.empty()) {
            std::vector<std::string> val;
            isIntWildcard ? val = {paramString, std::to_string(otherA)} : val = {std::to_string(otherA), paramString};
            res.push_back(val);
            intIntCache[val] = true;
            if (isEarlyReturn) {
                return res;
            }
        }
    }

    return res;
}

bool AffectsHandler::checkModifiedAssignReadCall(Ent commonVariable, StmtNum currentLine) {

    unordered_set<Ent>& entitiesModifiedOnCurrentLine = modifiesStorage->getRightItems(currentLine);

    // if an assignment, read, or procedure call, we check if the entitiesModifiedOnCurrentLine is the same as
    // commonVariables
    Stmt stmtType = stmtStorage->getStatementType(currentLine);

    if (stmtType == AppConstants::ASSIGN || stmtType == AppConstants::READ || stmtType == AppConstants::CALL) {

        if (entitiesModifiedOnCurrentLine.find(commonVariable) != entitiesModifiedOnCurrentLine.end()) {
            return true;
        }
    }
    return false;
}

bool AffectsHandler::checkCanReach(StmtNum a1, StmtNum a2, ProcName proc, Ent commonVariable) {

    // if either is not an assign statements, should also just return nothing already
    if (!procAssignStmtStorage->checkProcedure(proc, a1) || !procAssignStmtStorage->checkProcedure(proc, a2)) {
        return false;
    }

    if (commonVariable == "") {
        return false;
    }

    std::deque<std::pair<StmtNum, StmtNum>> queue;
    std::unordered_set<std::pair<StmtNum, StmtNum>, hashFunctionAffectsT> seen;
    const auto& graph = cfgStorage->getGraph(proc);

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
        else if (curr.first != AppConstants::DUMMY_NODE && checkModifiedAssignReadCall(commonVariable, curr.second)) {
            continue;
        }
        else if (seen.find(curr) != seen.end()) {
            continue;
        }

        seen.insert(curr);

        if (graph.find(curr.second) == graph.end()) {
            continue;
        }

        const auto& allRelatives = graph.at(curr.second);
        if (allRelatives.find(AppConstants::CHILDREN) == allRelatives.end()) {
            continue;
        }

        const std::unordered_set<StmtNum>& children = allRelatives.at(AppConstants::CHILDREN);
        for (StmtNum child : children) {
            queue.push_back({curr.second, child});
        }
    }
    return false;
}

void AffectsHandler::clearCache() {
    intIntCache.clear();
    intWildcardCache.clear();
    wildcardIntCache.clear();

    intIntTransitiveCache.clear();
    intWildcardTransitiveCache.clear();
    wildcardIntTransitiveCache.clear();
    procAffectsGraphMap.clear();
}
