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

std::vector<std::vector<std::string>> UsesStorage::getUsesAllWhileStatements() {
    return getUsesAllStatements("while");
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllWhileStatementsGivenProcedure(ProcedureName name) {
    return getUsesAllStatementsGivenProcedure("while", name);
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllWhileStatementsGivenEntity(Ent entity) {
    return getUsesAllStatementsGivenEntity("while", entity);
}

//format: {StmtNum, entity}
std::vector<std::vector<std::string>> UsesStorage::getUsesAllCallStatements_format1() {
    std::vector<std::vector<std::string>> res;
    for (auto i : call_callee) {
        std::unordered_set<std::string> entities = getAllEntitiesUsed(i.second);
        for (Ent e: entities) {
            std::vector<std::string> curr = {std::to_string(i.first), e};
            res.push_back(curr);
        }
    }
    return res;
}

//format: {StmtNum, procedureName}
std::vector<std::vector<std::string>> UsesStorage::getUsesAllCallStatements_format2() {
    std::vector<std::vector<std::string>> res;
    for (auto i : call_callee) {
        std::vector<std::string> curr = {std::to_string(i.first), i.second};
        res.push_back(curr);
    }
    return res;
}

//format: {StmtNum, entity}
std::vector<std::vector<std::string>> UsesStorage::getUsesAllCallStatementsGivenProcedure_format1(ProcedureName name) {
    std::vector<std::vector<std::string>> res;
    for (auto i : procName_stmtType_stmtNum[name]["call"]) {
        std::string callee = call_callee[i];
        std::unordered_set<std::string> entities = getAllEntitiesUsed(callee);

        for (Ent e : entities) {
            std::vector<std::string> curr = {std::to_string(i), e};
            res.push_back(curr);
        }
    }
    return res;
}

//format: {StmtNum, calleeName}
std::vector<std::vector<std::string>> UsesStorage::getUsesAllCallStatementsGivenProcedure_format2(ProcedureName name) {
    std::vector<std::vector<std::string>> res;
    for (auto i : procName_stmtType_stmtNum[name]["call"]) {
        std::string callee = call_callee[i];
        std::vector<std::string> curr = {std::to_string(i), callee};
        res.push_back(curr);
    }
    return res;
}

//format: {StmtNum, names of all procedure calls inside called procedure (recursive)}
std::vector<std::vector<std::string>> UsesStorage::getUsesAllCallStatementsGivenProcedure_format3(ProcedureName name) {
    std::unordered_set<std::pair<std::string, int>, usesCallsHashFunction> seen; //TODO: check with QPS that there is no cyclic issues
    std::vector<std::vector<std::string>> res;

    std::deque<std::pair<std::string, int>> queue;

    for (auto i : procName_stmtType_stmtNum[name]["call"]) {
        std::string callee = call_callee[i];
        queue.push_back(std::make_pair(callee, i));
    }

    while (!queue.empty()) {
        std::pair<std::string, int> curr = queue.front();
        queue.pop_front();

        if (seen.find(curr) != seen.end()) {
            continue;
        }
        seen.insert(curr);

        std::string procedureCalled = curr.first;
        int referenceLine = curr.second;
        std::vector<std::string> result = {std::to_string(referenceLine), procedureCalled};
        res.push_back(result);

        for (auto i : procName_stmtType_stmtNum[procedureCalled]["call"]) {
            std::string callee = call_callee[i];
            queue.push_back(std::make_pair(callee, referenceLine));
        }
    }
    return res;
}

// return {StmtNum, entity}
std::vector<std::vector<std::string>> UsesStorage::getUsesAllCallStatementsGivenEntity_format1(Ent entity) {
    std::vector<std::vector<std::string>> res;
    for (auto i : call_callee) {
        std::unordered_set<std::string> entities = getAllEntitiesUsed(i.second);
        if (entities.find(entity) != entities.end()) {
            std::vector<std::string> result = {std::to_string(i.first), entity};
            res.push_back(result);
        }
    }
    return res;
}

// return {StmtNum, procedureName}
std::vector<std::vector<std::string>> UsesStorage::getUsesAllCallStatementsGivenEntity_format2(Ent entity) {
    std::vector<std::vector<std::string>> res;
    for (auto i : call_callee) {
        std::unordered_set<std::string> entities = getAllEntitiesUsed(i.second);
        if (entities.find(entity) != entities.end()) {
            std::vector<std::string> result = {std::to_string(i.first), i.second};
            res.push_back(result);
        }
    }
    return res;
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllStatements() {
    std::vector<std::vector<std::string>> printStatements = getUsesAllPrintStatements();
    std::vector<std::vector<std::string>> assignStatements = getUsesAllAssignStatements();
    std::vector<std::vector<std::string>> ifStatements = getUsesAllIfStatements();
    std::vector<std::vector<std::string>> whileStatements = getUsesAllWhileStatements();
    std::vector<std::vector<std::string>> callStatements = getUsesAllCallStatements_format1();

    std::unordered_set<std::pair<std::string, int>, usesCallsHashFunction> seen; //TODO: check with QPS that there is no cyclic issues

    for (auto i : printStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }
    for (auto i : assignStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }
    for (auto i : ifStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }
    for (auto i : whileStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }
    for (auto i : callStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }

    std::vector<std::vector<std::string>> res;
    for (auto j : seen) {
        std::vector<std::string> curr = {std::to_string(j.second), j.first};
        res.push_back(curr);
    }
    return res;
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllStatementsGivenProcedure(ProcedureName name) {
    std::vector<std::vector<std::string>> printStatements = getUsesAllPrintStatementsGivenProcedure(name);
    std::vector<std::vector<std::string>> assignStatements = getUsesAllAssignStatementsGivenProcedure(name);
    std::vector<std::vector<std::string>> ifStatements = getUsesAllIfStatementsGivenProcedure(name);
    std::vector<std::vector<std::string>> whileStatements = getUsesAllWhileStatementsGivenProcedure(name);
    std::vector<std::vector<std::string>> callStatements = getUsesAllCallStatementsGivenProcedure_format1(name);

    std::unordered_set<std::pair<std::string, int>, usesCallsHashFunction> seen; //TODO: check with QPS that there is no cyclic issues

    for (auto i : printStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }
    for (auto i : assignStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }
    for (auto i : ifStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }
    for (auto i : whileStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }
    for (auto i : callStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }

    std::vector<std::vector<std::string>> res;
    for (auto j : seen) {
        std::vector<std::string> curr = {std::to_string(j.second), j.first};
        res.push_back(curr);
    }
    return res;
}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllStatementsGivenEntity(Ent entity) {
    std::vector<std::vector<std::string>> printStatements = getUsesAllPrintStatementsGivenEntity(entity);
    std::vector<std::vector<std::string>> assignStatements = getUsesAllAssignStatementsGivenEntity(entity);
    std::vector<std::vector<std::string>> ifStatements = getUsesAllIfStatementsGivenEntity(entity);
    std::vector<std::vector<std::string>> whileStatements = getUsesAllWhileStatementsGivenEntity(entity);
    std::vector<std::vector<std::string>> callStatements = getUsesAllCallStatementsGivenEntity_format1(entity);

    std::unordered_set<std::pair<std::string, int>, usesCallsHashFunction> seen; //TODO: check with QPS that there is no cyclic issues

    for (auto i: printStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }
    for (auto i: assignStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }
    for (auto i: ifStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }
    for (auto i: whileStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }
    for (auto i: callStatements) {
        int num = std::stoi(i[0]);
        std::pair<std::string, int> p = std::make_pair(i[1], num);
        seen.insert(p);
    }

    std::vector<std::vector<std::string>> res;
    for (auto j: seen) {
        std::vector<std::string> curr = {std::to_string(j.second), j.first};
        res.push_back(curr);
    }
    return res;
}

/*
std::vector<std::vector<std::string>> UsesStorage::getUsesAllProcedures() {

}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllProceduresGivenProcedure(ProcedureName name) {

}

std::vector<std::vector<std::string>> UsesStorage::getUsesAllProceduresGivenEntity(Ent e) {

}
*/
