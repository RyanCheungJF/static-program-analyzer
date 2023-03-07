#include "AffectsHandler.h"

AffectsHandler::AffectsHandler(std::shared_ptr<CFGStorage> cfgStorage,
                               std::shared_ptr<StmtStorage> stmtStorage,
                               std::shared_ptr<ProcedureStorage> procStorage,
                               std::shared_ptr<ModifiesUsesStorage> modifiesStorage,
                               std::shared_ptr<ModifiesUsesStorage> usesStorage,
                               bool isTransitive) {
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
    bool isWildCardParam1 = paramType1 == ParameterType::WILDCARD || paramType1 == ParameterType::ASSIGN;
    bool isWildCardParam2 = paramType2 == ParameterType::WILDCARD || paramType2 == ParameterType::ASSIGN;

    if (isFixedIntParam1) {
        if (isFixedIntParam2) {
            return handleIntInt(stoi(param1.getValue()), stoi(param2.getValue()));
        }
        else if (isWildCardParam2) {
            return handleIntWildcard(stoi(param1.getValue()));
        }
    }
    else if (isWildCardParam1) {
        if (isFixedIntParam2) {
            return handleWildcardInt(stoi(param2.getValue()));
        }
        else if (isWildCardParam2) {
            return handleWildcardWildcard();
        }
    }
    return std::vector<std::vector<std::string>>();
}


//TODO: try a case with Affects(1, 1) where line 1 is v = v + 1
std::vector<std::vector<std::string>> AffectsHandler::handleIntInt(StmtNum a1, StmtNum a2) {
    std::string paramString1 = std::to_string(a1);
    std::string paramString2 = std::to_string(a2);
    ProcName proc1 = procStorage->getProcedure(a1);
    ProcName proc2 = procStorage->getProcedure(a2);
    std::vector<std::vector<std::string>> res;

    if (proc1 == "INVALID" || proc2 == "INVALID") {
        return res;
    } else if (proc1 != proc2) {
        return res;
    }

    std::unordered_set<Ent> variablesModifiedInA1 = modifiesStorage->getEnt(a1);
    std::unordered_set<Ent> variablesUsedInA2 = usesStorage->getEnt(a2);
    std::unordered_set<Ent> commonVariables = getCommonVariables(variablesModifiedInA1, variablesUsedInA2);
    if (commonVariables.empty()) {
        return res;
    }

    std::unordered_set<StmtNum> controlFlowPath = getControlFlowPathIntInt(a1, a2, proc1);
    if (controlFlowPath.empty()) {
        return res;
    }

    std::unordered_set<Ent> variablesModifiedInPath = getVariablesModifiedInControlFlowPath(controlFlowPath);
    for (Ent e : commonVariables) {
        if (variablesModifiedInPath.find(e) != variablesModifiedInPath.end()) {
            return res;
        }
    }

    res.push_back({paramString1, paramString2});
    return res;
}

std::vector<std::vector<std::string>> AffectsHandler::handleWildcardInt(StmtNum a2) {
    std::string paramString2 = std::to_string(a2);
    ProcName proc = procStorage->getProcedure(a2);
    std::vector<std::vector<std::string>> res;

    if (proc == "INVALID") {
        return res;
    }
    std::unordered_set<StmtNum> statements = procStorage->getProcedureStatementNumbers(proc);
    std::unordered_set<StmtNum> assignStatements;
    for (StmtNum num : statements) {
        if (stmtStorage->getStatementType(num).find(AppConstants::ASSIGN) != stmtStorage->getStatementType(num).end()) {
            assignStatements.insert(num);
        }
    }

    std::unordered_set<Ent> variablesUsedInA2 = usesStorage->getEnt(a2);
    for (StmtNum a1 : assignStatements) {
        std::unordered_set<Ent> variablesModifiedInA1 = modifiesStorage->getEnt(a1);
        std::unordered_set<Ent> commonVariables = getCommonVariables(variablesModifiedInA1, variablesUsedInA2);
        if (commonVariables.empty()) {
            continue;
        }

        std::unordered_set<StmtNum> controlFlowPath = getControlFlowPathIntInt(a1, a2, proc);
        if (controlFlowPath.empty()) {
            continue;
        }

        std::unordered_set<Ent> variablesModifiedInPath = getVariablesModifiedInControlFlowPath(controlFlowPath);
        if (a1 == a2) {
            for (Ent e : commonVariables) {
                if (variablesModifiedInPath.find(e) != variablesModifiedInPath.end()) {
                    res.push_back({std::to_string(a1), paramString2});
                    break;
                }
            }
            continue;
        }

        bool isModified = false;
        for (Ent e : commonVariables) {
            if (variablesModifiedInPath.find(e) != variablesModifiedInPath.end()) {
                isModified = true;
            }
        }

        if (isModified) {
            continue;
        } else {
            res.push_back({std::to_string(a1), paramString2});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> AffectsHandler::handleIntWildcard(StmtNum a1) {
    std::string paramString1 = std::to_string(a1);
    ProcName proc = procStorage->getProcedure(a1);
    std::vector<std::vector<std::string>> res;

    if (proc == "INVALID") {
        return res;
    }
    std::unordered_set<StmtNum> statements = procStorage->getProcedureStatementNumbers(proc);
    std::unordered_set<StmtNum> assignStatements;
    for (StmtNum num : statements) {
        if (stmtStorage->getStatementType(num).find(AppConstants::ASSIGN) != stmtStorage->getStatementType(num).end()) {
            assignStatements.insert(num);
        }
    }

    std::unordered_set<Ent> variablesModifiedInA1 = modifiesStorage->getEnt(a1);
    for (StmtNum a2 : assignStatements) {
        std::unordered_set<Ent> variablesUsedInA2 = usesStorage->getEnt(a2);
        std::unordered_set<Ent> commonVariables = getCommonVariables(variablesModifiedInA1, variablesUsedInA2);
        if (commonVariables.empty()) {
            continue;
        }

        std::unordered_set<StmtNum> controlFlowPath = getControlFlowPathIntInt(a1, a2, proc);
        if (controlFlowPath.empty()) {
            continue;
        }

        std::unordered_set<Ent> variablesModifiedInPath = getVariablesModifiedInControlFlowPath(controlFlowPath);
        if (a1 == a2) {
            for (Ent e : commonVariables) {
                if (variablesModifiedInPath.find(e) != variablesModifiedInPath.end()) {
                    res.push_back({paramString1, std::to_string(a2)});
                    break;
                }
            }
            continue;
        }

        bool isModified = false;
        for (Ent e : commonVariables) {
            if (variablesModifiedInPath.find(e) != variablesModifiedInPath.end()) {
                isModified = true;
            }
        }

        if (isModified) {
            continue;
        } else {
            res.push_back({paramString1, std::to_string(a2)});
        }
    }
    return res;
}

std::vector<std::vector<std::string>> AffectsHandler::handleWildcardWildcard() {
    std::vector<std::vector<std::string>> res;

    std::unordered_set<ProcName> allProcedures = procStorage->getProcNames();
    for (ProcName proc : allProcedures) {
        std::unordered_set<StmtNum> statements = procStorage->getProcedureStatementNumbers(proc);
        std::unordered_set<StmtNum> assignStatements;
        for (StmtNum num : statements) {
            if (stmtStorage->getStatementType(num).find(AppConstants::ASSIGN) != stmtStorage->getStatementType(num).end()) {
                assignStatements.insert(num);
            }
        }

        for (StmtNum a1 : assignStatements) {
            std::vector<std::vector<std::string>> temp = handleIntWildcard(a1);
            res.reserve(res.size() + temp.size());
            std::move(temp.begin(), temp.end(), std::inserter(res, res.end()));
            temp.clear(); //todo: this code might have memory management issues
        }
    }
    return res;
}





//helper functions
std::unordered_set<StmtNum> AffectsHandler::getControlFlowPathIntInt(StmtNum a1, StmtNum a2, ProcName proc) {

    std::unordered_set<StmtNum> res;
    if (a1 == a2) {
        res.insert(a1);
        return res;
    }

    std::deque<std::pair<std::unordered_set<StmtNum>, StmtNum>> queue;
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph = cfgStorage->getGraph(proc);

    std::pair<std::unordered_set<StmtNum>, StmtNum> p = {{}, a1}; // TOOO: WILL THIS LEAD TO AN ERROR? THE WAY WE DECLARE PAIR
    queue.push_back(p);
    while (!queue.empty()) {
        std::pair<std::unordered_set<StmtNum>, StmtNum> curr = queue.front();
        queue.pop_front();
        if (curr.second == a2) {
            std::unordered_set<StmtNum> path = curr.first;
            path.erase(a1);
            res.insert(path.begin(), path.end());
            continue;
        } else if (curr.first.find(curr.second) != curr.first.end()) {
            continue;
        }

        curr.first.insert(curr.second);
        std::unordered_set<StmtNum> children = graph[curr.second][AppConstants::CHILDREN];
        for (StmtNum child : children) {
            std::unordered_set<StmtNum> nxtPath (curr.first);
            queue.push_back({nxtPath, child});
        }
    }
    return res;
}

std::unordered_set<Ent> AffectsHandler::getVariablesModifiedInControlFlowPath(std::unordered_set<StmtNum> controlFlowPath) {
    std::unordered_set<Ent> variablesModifiedInPath;
    for (StmtNum num : controlFlowPath) {
        std::unordered_set<Stmt> stmtTypes = stmtStorage->getStatementType(num);

        // assignment, read, procedure call
        if (stmtTypes.find(AppConstants::ASSIGN) != stmtTypes.end() ||
            stmtTypes.find(AppConstants::READ) != stmtTypes.end() ||
            stmtTypes.find(AppConstants::CALL) != stmtTypes.end()
                ) {
            std::unordered_set<Ent> variablesModifiedInCurrentLine = modifiesStorage->getEnt(num);
            variablesModifiedInPath.insert(variablesModifiedInCurrentLine.begin(),variablesModifiedInCurrentLine.end());
        }
    }
    return variablesModifiedInPath;
}

std::unordered_set<Ent> AffectsHandler::getCommonVariables(std::unordered_set<Ent> variablesModifiedInA1,
                                                           std::unordered_set<Ent> variablesUsedInA2) {

    std::unordered_set<Ent> commonVariables;
    for (Ent e : variablesModifiedInA1) { //TODO: area for optimisation in Milestone 3. use the smaller set
        if (variablesUsedInA2.find(e) != variablesUsedInA2.end()) {
            commonVariables.insert(e);
        }
    }
    return commonVariables;
}
