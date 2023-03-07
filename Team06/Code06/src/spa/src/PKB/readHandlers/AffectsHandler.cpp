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

std::vector<std::vector<std::string>> AffectsHandler::handleIntInt(Parameter param1, Parameter param2) {
    std::string paramString1 = param1.getValue();
    std::string paramString2 = param2.getValue();
    StmtNum a1 = stoi(paramString1);
    StmtNum a2 = stoi(paramString2);
    ProcName proc1 = procStorage->getProcedure(stoi(paramString1));
    ProcName proc2 = procStorage->getProcedure(stoi(paramString2));
    std::vector<std::vector<std::string>> res;

    if (proc1 == "INVALID" || proc2 == "INVALID") {
        return res;
    } else if (proc1 != proc2) {
        return res;
    }

    std::unordered_set<Ent> variablesModifiedInA1 = modifiesStorage->getEnt(a1);
    std::unordered_set<Ent> variablesUsedInA2 = usesStorage->getEnt(a2);
    std::unordered_set<Ent> commonVariables;
    for (Ent e : variablesModifiedInA1) { //TODO: area for optimisation in Milestone 3. use the smaller set
        if (variablesUsedInA2.find(e) != variablesUsedInA2.end()) {
            commonVariables.insert(e);
        }
    }
    if (commonVariables.empty()) {
        return res;
    }

    std::unordered_set<StmtNum> controlFlowPath = getControlFlowPathIntInt(a1, a2, proc1);
    if (controlFlowPath.empty()) {
        return res;
    }

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

    for (Ent e : commonVariables) {
        if (variablesModifiedInPath.find(e) != variablesModifiedInPath.end()) {
            return res;
        }
    }

    res.push_back({a1, a2});
    return res;


}

std::unordered_set<StmtNum> AffectsHandler::getControlFlowPathIntInt(StmtNum a1, StmtNum a2, ProcName proc) {

    std::unordered_set<StmtNum> res;
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
