#include "AffectsHandler.h"

AffectsHandler::AffectsHandler(std::shared_ptr<CFGStorage> cfgStorage, std::shared_ptr<StmtStorage> stmtStorage,
                               std::shared_ptr<ProcedureStorage> procStorage,
                               std::shared_ptr<ModifiesUsesStorage> modifiesStorage,
                               std::shared_ptr<ModifiesUsesStorage> usesStorage,
                               std::shared_ptr<FollowsParentStorage> parentTStorage, bool isTransitive) {
    this->cfgStorage = cfgStorage;
    this->stmtStorage = stmtStorage;
    this->procStorage = procStorage;
    this->modifiesStorage = modifiesStorage;
    this->usesStorage = usesStorage;
    this->parentTStorage = parentTStorage;
    this->isTransitive = isTransitive;
}

std::vector<std::vector<std::string>> AffectsHandler::handle(Parameter param1, Parameter param2) {

    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    ParameterType paramType1 = param1.getType();
    ParameterType paramType2 = param2.getType();

    bool isFixedIntParam1 = paramType1 == ParameterType::FIXED_INT;
    bool isFixedIntParam2 = paramType2 == ParameterType::FIXED_INT;
    bool isWildCardParam1 = paramType1 == ParameterType::WILDCARD || paramType1 == ParameterType::ASSIGN;
    bool isWildCardParam2 = paramType2 == ParameterType::WILDCARD || paramType2 == ParameterType::ASSIGN;

    if (isTransitive) {
        return handleTransitive(param1.getValue(), param2.getValue(), isFixedIntParam1, isFixedIntParam2,
                                isWildCardParam1, isWildCardParam2);
    }
    else {
        return handleNonTransitive(param1.getValue(), param2.getValue(), isFixedIntParam1, isFixedIntParam2,
                                   isWildCardParam1, isWildCardParam2);
    }
}

std::vector<std::vector<std::string>> AffectsHandler::handleIntInt(StmtNum a1, StmtNum a2) {
    std::string paramString1 = std::to_string(a1);
    std::string paramString2 = std::to_string(a2);
    ProcName proc1 = procStorage->getProcedure(a1);
    ProcName proc2 = procStorage->getProcedure(a2);
    std::vector<std::vector<std::string>> res;

    if (proc1 == AppConstants::PROCEDURE_DOES_NOT_EXIST || proc2 == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }
    else if (proc1 != proc2) {
        return res;
    }

    std::unordered_set<StmtNum> statements = procStorage->getProcedureStatementNumbers(proc1);
    std::unordered_set<StmtNum> assignStatements = getAssignStatements(statements);
    if (assignStatements.find(a1) == assignStatements.end() || assignStatements.find(a2) == assignStatements.end()) {
        return res;
    }

    std::unordered_set<Ent> variablesModifiedInA1 = modifiesStorage->getEnt(a1);
    std::unordered_set<Ent> variablesUsedInA2 = usesStorage->getEnt(a2);
    std::unordered_set<Ent> commonVariables = getCommonVariables(variablesModifiedInA1, variablesUsedInA2);
    if (commonVariables.empty()) {
        return res;
    }

    std::unordered_set<StmtNum> controlFlowPath = getControlFlowPathIntInt(a1, a2, proc1);

    if (controlFlowPath.empty() && !checkDirectlyAfterEachOther(a1, a2) && !checkHaveCommonWhileLoop(a1, a2)) {
        return res;
    }

    std::unordered_set<Ent> variablesModifiedInPath = getVariablesModifiedInControlFlowPath(controlFlowPath);
    bool isModified = isModifiedInControlFlowPath(commonVariables, variablesModifiedInPath);
    if (isModified) {
        return res;
    }

    res.push_back({paramString1, paramString2});
    return res;
}

std::vector<std::vector<std::string>> AffectsHandler::handleWildcardInt(StmtNum a2) {
    return nonTransitiveOneIntOneWildcard(AppConstants::NOT_USED_FIELD, a2);
}

std::vector<std::vector<std::string>> AffectsHandler::handleIntWildcard(StmtNum a1) {
    return nonTransitiveOneIntOneWildcard(a1, AppConstants::NOT_USED_FIELD);
}

std::vector<std::vector<std::string>> AffectsHandler::handleWildcardWildcard() {
    std::vector<std::vector<std::string>> res;

    std::unordered_set<ProcName> allProcedures = procStorage->getProcNames();
    for (ProcName proc : allProcedures) {
        std::unordered_set<StmtNum> statements = procStorage->getProcedureStatementNumbers(proc);
        std::unordered_set<StmtNum> assignStatements = getAssignStatements(statements);

        for (StmtNum a1 : assignStatements) {
            std::vector<std::vector<std::string>> temp = handleIntWildcard(a1);
            res.reserve(res.size() + temp.size());
            std::move(temp.begin(), temp.end(), std::inserter(res, res.end()));
            temp.clear(); // todo: this code might have memory management issues
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

    std::unordered_map<StmtNum, unordered_set<StmtNum>> hashmap = buildAffectsGraph(false);
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
    std::unordered_map<StmtNum, unordered_set<StmtNum>> hashmap = buildAffectsGraph(false);

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

    //remove duplicates
    std::unordered_set<std::pair<StmtNum, StmtNum>, hashFunctionAffectsT> temp;
    for (std::tuple<StmtNum, StmtNum, StmtNum> curr : seen) {
        temp.insert({get<0>(curr), get<2>(curr)});
    }

    for (std::pair<StmtNum, StmtNum> p : temp) {
        res.push_back({std::to_string(p.first), std::to_string(p.second)});
    }

//    for (std::tuple<StmtNum, StmtNum, StmtNum> curr : seen) {
//        res.push_back({std::to_string(get<0>(curr)), std::to_string(get<2>(curr))});
//    }
    return res;
}

// helper functions
std::unordered_set<StmtNum> AffectsHandler::getControlFlowPathIntInt(StmtNum a1, StmtNum a2, ProcName proc) {

    std::unordered_set<StmtNum> res;
    std::deque<std::pair<std::unordered_set<StmtNum>, StmtNum>> queue;
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph =
        cfgStorage->getGraph(proc);

    std::pair<std::unordered_set<StmtNum>, StmtNum> p = {{}, a1};
    queue.push_back(p);
    while (!queue.empty()) {
        std::pair<std::unordered_set<StmtNum>, StmtNum> curr = queue.front();
        queue.pop_front();
        if (curr.second == a2 && !(curr.first.empty())) {
            std::unordered_set<StmtNum> path = curr.first;
            path.erase(a1);
            res.insert(path.begin(), path.end());
            continue;
        }
        else if (curr.first.find(curr.second) != curr.first.end()) {
            continue;
        }

        curr.first.insert(curr.second);
        std::unordered_set<StmtNum> children = graph[curr.second][AppConstants::CHILDREN];
        for (StmtNum child : children) {
            std::unordered_set<StmtNum> nxtPath(curr.first);
            queue.push_back({nxtPath, child});
        }
    }
    return res;
}

std::unordered_set<Ent>
AffectsHandler::getVariablesModifiedInControlFlowPath(std::unordered_set<StmtNum> controlFlowPath) {
    std::unordered_set<Ent> variablesModifiedInPath;
    for (StmtNum num : controlFlowPath) {
        std::unordered_set<Stmt> stmtTypes = stmtStorage->getStatementType(num);

        // assignment, read, procedure call
        if (stmtTypes.find(AppConstants::ASSIGN) != stmtTypes.end() ||
            stmtTypes.find(AppConstants::READ) != stmtTypes.end() ||
            stmtTypes.find(AppConstants::CALL) != stmtTypes.end()) {
            std::unordered_set<Ent> variablesModifiedInCurrentLine = modifiesStorage->getEnt(num);
            variablesModifiedInPath.insert(variablesModifiedInCurrentLine.begin(),
                                           variablesModifiedInCurrentLine.end());
        }
    }
    return variablesModifiedInPath;
}

std::unordered_set<Ent> AffectsHandler::getCommonVariables(std::unordered_set<Ent> variablesModifiedInA1,
                                                           std::unordered_set<Ent> variablesUsedInA2) {

    std::unordered_set<Ent> commonVariables;
    for (Ent e : variablesModifiedInA1) { // O(1) since there is really only 1 element
        if (variablesUsedInA2.find(e) != variablesUsedInA2.end()) {
            commonVariables.insert(e);
        }
    }

//    for (Ent e : variablesUsedInA2) { // O(1) since there is really only 1 element
//        if (variablesModifiedInA1.find(e) != variablesModifiedInA1.end()) {
//            commonVariables.insert(e);
//        }
//    }
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
            return handleWildcardWildcard();
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

std::unordered_map<StmtNum, unordered_set<StmtNum>> AffectsHandler::buildAffectsGraph(bool isInverted) {

    // build the hop graph
    std::vector<std::vector<std::string>> allValidAffects = handleWildcardWildcard();
    std::unordered_map<StmtNum, unordered_set<StmtNum>> hashmap;
    for (std::vector<std::string> p : allValidAffects) {
        isInverted ? hashmap[stoi(p[1])].insert(stoi(p[0])) : hashmap[stoi(p[0])].insert(stoi(p[1]));
    }
    return hashmap;
}

// TODO: area for optimisation. get this at compile time
std::unordered_set<StmtNum> AffectsHandler::getAssignStatements(std::unordered_set<StmtNum> allProcStatements) {

    std::unordered_set<StmtNum> assignStatements;
    for (StmtNum num : allProcStatements) {
        std::unordered_set<Stmt> statementTypes = stmtStorage->getStatementType(num);
        if (statementTypes.find(AppConstants::ASSIGN) != statementTypes.end()) {
            assignStatements.insert(num);
        }
    }
    return assignStatements;
}

bool AffectsHandler::isModifiedInControlFlowPath(std::unordered_set<Ent> commonVariables,
                                                 std::unordered_set<Ent> variablesModifiedInPath) {
    for (Ent e : commonVariables) {
        if (variablesModifiedInPath.find(e) != variablesModifiedInPath.end()) {
            return true;
        }
    }
    return false;
}

std::vector<std::vector<std::string>> AffectsHandler::bfsTraversalOneWildcard(StmtNum a1, StmtNum a2) {
    bool isIntWildcard = a2 == AppConstants::NOT_USED_FIELD;
    std::unordered_map<StmtNum, unordered_set<StmtNum>> hashmap = buildAffectsGraph(!isIntWildcard);
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
            } else {
                queue.push_back({num, curr.first});
            }
        }
    }

    // remove potential duplicate entries
    std::unordered_set<std::pair<StmtNum, StmtNum>, hashFunctionAffectsT> temp;
    for (std::pair<StmtNum, StmtNum> p : seen) {
        isIntWildcard ? temp.insert({a1, p.second})
                      : temp.insert({p.first, a2});
    }

    std::vector<std::vector<std::string>> res;
    std::string paramString = (isIntWildcard ? std::to_string(a1) : std::to_string(a2));
    for (std::pair<StmtNum, StmtNum> p : temp) {
        isIntWildcard ? res.push_back({paramString, std::to_string(p.second)})
                      : res.push_back({std::to_string(p.first), paramString});
    }
    return res;
}

bool AffectsHandler::checkDirectlyAfterEachOther(StmtNum a1, StmtNum a2) {

    //if they are not consecutive
    if (!(a1 + 1 == a2 || a1 - 1 == a2)) {
        return false;
    }

    // means they are consecutive in terms of numbers, but might still be part of different if-else branches

    //given they are not in a while loop, a2 MUST come after a1
    if (a2 < a1) {
        return false;
    }

    //the 2 lines are in an if-else block
    /*
     * case1
     * if (...) then {
     *     a1;
     * } else {
     *     a2;
     * }
     *
     * case2 [WHICH SHOULD NOT BE POSSIBLE IF THEY ARE NOT IN WHILE LOOPS TO BEGIN WITH]
     * if (...) then {
     *     a2;
     * } else {
     *     a1;
     * }
     */
    std::unordered_set<Stmt> oneLineBeforeA1Stmt = stmtStorage->getStatementType(a1 - 1);
    std::unordered_set<Stmt> twoLinesBeforeA2Stmt = stmtStorage->getStatementType(a2 - 2);

    std::unordered_set<Stmt> twoLinesBeforeA1Stmt = stmtStorage->getStatementType(a1 - 2);
    std::unordered_set<Stmt> oneLineBeforeA2Stmt = stmtStorage->getStatementType(a2 - 1);

    if ((oneLineBeforeA1Stmt.find(AppConstants::IF) != oneLineBeforeA1Stmt.end()) &&
        (twoLinesBeforeA2Stmt.find(AppConstants::IF) != twoLinesBeforeA2Stmt.end())) {
        return false;
    }

    if ((twoLinesBeforeA1Stmt.find(AppConstants::IF) != twoLinesBeforeA1Stmt.end()) &&
        (oneLineBeforeA2Stmt.find(AppConstants::IF) != oneLineBeforeA2Stmt.end())) {
        return false;
    }

    return true;
}

std::vector<std::vector<std::string>> AffectsHandler::nonTransitiveOneIntOneWildcard(StmtNum a1input, StmtNum a2input) {
    bool isIntWildcard = (a2input == AppConstants::NOT_USED_FIELD);
    std::string paramString = isIntWildcard ? std::to_string(a1input) : std::to_string(a2input);
    StmtNum currA = isIntWildcard ? a1input : a2input;
    ProcName proc = procStorage->getProcedure(currA);
    std::vector<std::vector<std::string>> res;

    if (proc == AppConstants::PROCEDURE_DOES_NOT_EXIST) {
        return res;
    }

    std::unordered_set<StmtNum> statements = procStorage->getProcedureStatementNumbers(proc);
    std::unordered_set<StmtNum> assignStatements = getAssignStatements(statements);
    if (assignStatements.find(currA) == assignStatements.end()) {
        return res;
    }

    std::unordered_set<Ent> variablesInCurrA = isIntWildcard ? modifiesStorage->getEnt(currA) : usesStorage->getEnt(currA);
    for (StmtNum otherA : assignStatements) {
        if (procStorage->getProcedure(currA) != procStorage->getProcedure(otherA)) {
            continue;
        }

        std::unordered_set<Ent> variablesInOtherA = isIntWildcard ? usesStorage->getEnt(otherA) : modifiesStorage->getEnt(otherA);
        std::unordered_set<Ent> commonVariables = getCommonVariables(variablesInCurrA, variablesInOtherA);
        if (commonVariables.empty()) {
            continue;
        }

        std::unordered_set<StmtNum> controlFlowPath = isIntWildcard ? getControlFlowPathIntInt(currA, otherA, proc) :
                                                      getControlFlowPathIntInt(otherA, currA, proc);

        if (controlFlowPath.empty()) {
            if (isIntWildcard && !checkDirectlyAfterEachOther(currA, otherA) && !checkHaveCommonWhileLoop(currA, otherA)) {
                continue;
            } else if (!isIntWildcard && !checkDirectlyAfterEachOther(otherA, currA) && !checkHaveCommonWhileLoop(otherA, currA)) {
                continue;
            }
        }

        std::unordered_set<Ent> variablesModifiedInPath = getVariablesModifiedInControlFlowPath(controlFlowPath);
        if ((currA == otherA) && variablesModifiedInPath.empty() &&
            (commonVariables.size() == 1)) { // O(1) since there is really only 1 element
            isIntWildcard ? res.push_back({paramString, std::to_string(otherA)}) : res.push_back({std::to_string(otherA), paramString});
            continue;
        }

        bool isModified = isModifiedInControlFlowPath(commonVariables, variablesModifiedInPath);
        if (isModified) {
            continue;
        }
        else {
            isIntWildcard ? res.push_back({paramString, std::to_string(otherA)}) : res.push_back({std::to_string(otherA), paramString});
        }
    }
    return res;
}

bool AffectsHandler::checkHaveCommonWhileLoop(StmtNum a1, StmtNum a2) {

    std::unordered_set<StmtNum> a1Parents = parentTStorage->getLeftWildcard(a1);
    std::unordered_set<StmtNum> a2Parents = parentTStorage->getLeftWildcard(a2);
    for (StmtNum n : a1Parents) {
        if (a2Parents.find(n) != a2Parents.end()) {

            unordered_set<Stmt> stmtType = stmtStorage->getStatementType(n);
            if (stmtType.find(AppConstants::WHILE) != stmtType.end()) {
                return true;
            }
        }
    }
    return false;
}
