#include "UsesStorage.h"

/* Helper Functions */
std::vector<std::vector<std::string>> UsesStorage::getUsesAllStatements(StmtType type) {
    std::vector<std::vector<std::string>> res; // {stmtNum, var}

    for (auto i : procName_stmtType_stmtNum) {
        for (auto stmtNum : i.second[type]) {
            for (Ent e : stmtNum_ent[stmtNum]) {
                std::vector<std::string> curr = {std::to_string(stmtNum), e};
                res.push_back(curr);
            }
        }
    }
    return res;
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllStatementsGivenProcedure(StmtType type, ProcedureName name) {
    std::vector<std::vector<std::string>> res; // {stmtNum, var}

    for (auto stmtNum : procName_stmtType_stmtNum[name][type]) {
        for (Ent e : stmtNum_ent[stmtNum]) {
            std::vector<std::string> curr = {std::to_string(stmtNum), e};
            res.push_back(curr);
        }
    }
    return res;
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllStatementsGivenEntity(StmtType type, Ent entity) {
    std::vector<std::vector<std::string>> res; // {stmtNum, var}

    for (auto i : procName_stmtType_stmtNum) {
        for (auto stmtNum : i.second[type]) {
            for (Ent e : stmtNum_ent[stmtNum]) {
                if (e == entity) {
                    std::vector<std::string> curr = {std::to_string(stmtNum), e};
                    res.push_back(curr);
                }
            }
        }
    }
    return res;
}

//TODO: make global constants for assign, if, while, print, call, etc. standardise with SP side
void UsesStorage::writeUses(ProcedureName name, StmtType type, StmtNum num, Ent ent) {
    procName_stmtType_stmtNum[name][type].insert(num);
    stmtNum_procName_stmtType[num] = std::make_pair(name, type);
//    procedure_entities[name].insert(ent);
    stmtNum_ent[num].insert(ent);
}

void UsesStorage::writeUsesCall(ProcedureName caller, ProcedureName callee, StmtNum num) {
    procName_stmtType_stmtNum[caller]["call"].insert(num);
//    caller_callee[caller].insert(callee);
    call_callee[num] = callee;
    return;
}


std::unordered_set<std::string> UsesStorage::getAllEntitiesUsed(ProcedureName name) {
    std::unordered_set<std::string> seen; //TODO: check with QPS that there is no cyclic issues
    std::unordered_set<std::string> res;
    std::deque<std::string> queue;
    queue.push_back(name);

    while (!queue.empty()) {
        std::string proc = queue.front();
        queue.pop_front();
        if (seen.find(proc) != seen.end()) {
            continue;
        }

        for (auto p : procName_stmtType_stmtNum[proc]) {
            std::string type = p.first;
            auto nums = p.second;

            for (int num : nums) {
                if (type == "call") {
                    queue.push_back(call_callee[num]);
                } else {
//                    for (Ent e: stmtNum_ent[num]) {
//                        res.insert(e);
//                    }
                    res.insert(stmtNum_ent[num].begin(), stmtNum_ent[num].end());
                }
            }
        }

        seen.insert(proc);
    }
    return res;
}

bool UsesStorage::checkUses(StmtNum num, Ent e) {

    // stmtNum not in source code
    if (stmtNum_procName_stmtType.find(num) == stmtNum_procName_stmtType.end()) {
        return false;
    } else if (call_callee.find(num) != call_callee.end()) { // num is a call statement
        std::string callee = call_callee[num];
        std::unordered_set<std::string> res = getAllEntitiesUsed(callee);
        return res.find(e) != res.end();
    } else {
        return stmtNum_ent[num].find(e)  != stmtNum_ent[num].end();
    }
}

bool UsesStorage::checkUses(ProcedureName name, Ent e) {
    // procedure not in source code
    if (procName_stmtType_stmtNum.find(name) == procName_stmtType_stmtNum.end()) {
        return false;
    }

    std::unordered_set<std::string> allEntities = getAllEntitiesUsed(name);
    return allEntities.find(e) != allEntities.end();
}


std::vector<std::vector<std::string>> UsesStorage::getUsesAllPrintStatements() {
    return getUsesAllStatements("print");
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllPrintStatementsGivenProcedure(ProcedureName name) {
    return getUsesAllStatementsGivenProcedure("print", name);
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllPrintStatementsGivenEntity(Ent entity) {
    return getUsesAllStatementsGivenEntity("print", entity);
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllAssignStatements() {
    return getUsesAllStatements("assign");
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllAssignStatementsGivenProcedure(ProcedureName name) {
    return getUsesAllStatementsGivenProcedure("assign", name);
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllAssignStatementsGivenEntity(Ent entity) {
    return getUsesAllStatementsGivenEntity("assign", entity);
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllIfStatements() {
    return getUsesAllStatements("if");
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllIfStatementsGivenProcedure(ProcedureName name) {
    return getUsesAllStatementsGivenProcedure("if", name);
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllIfStatementsGivenEntity(Ent entity) {
    return getUsesAllStatementsGivenEntity("if", entity);
}

/*
std::vector<std::vector<std::string>> UsesStorage::getUsesStatementAll() {
    std::vector<std::vector<std::string>> res; // {stmtNum, var}

    for (auto i : procName_stmtType_stmtNum) {
        for (auto j : i.second) {
            std::string stmtType = j.first;

            for (StmtNum num: j.second) {
                if (stmtType == "call") {


                } else if (stmtType == "print") {

                } else if (stmtType == "assign") {

                } else if (stmtType == "if") {

                } else if (stmtType == "while") {

                }
            }
        }
    }
    return res;
}
*/
