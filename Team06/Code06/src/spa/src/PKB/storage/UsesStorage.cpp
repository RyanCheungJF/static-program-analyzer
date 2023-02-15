#include "UsesStorage.h"

void UsesStorage::writeUsesS(StmtNum num, std::unordered_set<Ent> entities) {
    stmtNum_entities[num].insert(entities.begin(), entities.end());
    for (Ent e : entities) {
        entities_stmtNum[e].insert(num);
    }
}

void UsesStorage::writeUsesP(ProcName name, std::unordered_set<Ent> entities) {
    procName_entities[name].insert(entities.begin(), entities.end());
    for (Ent e : entities) {
        entities_procName[e].insert(name);
    }
}

bool UsesStorage::exists(StmtNum num, Ent var) {
    if (stmtNum_entities.find(num) == stmtNum_entities.end()) return false;

    std::unordered_set<Ent> entities = stmtNum_entities[num];
    return entities.find(var) != entities.end();
}

bool UsesStorage::exists(ProcName proc, Ent var) {
    if (procName_entities.find(proc) == procName_entities.end()) return false;

    std::unordered_set<Ent> entities = procName_entities[proc];
    return entities.find(var) != entities.end();
}

std::unordered_set<Ent> UsesStorage::getEnt(StmtNum num) {
    if (stmtNum_entities.find(num) == stmtNum_entities.end()) {
        std::unordered_set<Ent> emptySet;
        return emptySet;
    }
    return stmtNum_entities[num];
}

std::unordered_set<Ent> UsesStorage::getEnt(ProcName name) {
    if (procName_entities.find(name) == procName_entities.end()) {
        std::unordered_set<Ent> emptySet;
        return emptySet;
    }
    return procName_entities[name];
}

std::unordered_set<StmtNum> UsesStorage::getStmtsFromEnt(Ent var) {
    if (entities_stmtNum.find(var) == entities_stmtNum.end()) {
        std::unordered_set<StmtNum> emptySet;
        return emptySet;
    }
    return entities_stmtNum[var];
}

std::unordered_set<ProcName> UsesStorage::getProcsFromEnt(Ent var) {
    if (entities_procName.find(var) == entities_procName.end()) {
        std::unordered_set<ProcName> emptySet;
        return emptySet;
    }
    return entities_procName[var];
}

std::pair<std::vector<StmtNum>, std::vector<std::string>> UsesStorage::getAllStmtEntPairs() {
    std::vector<StmtNum> stmtNums;
    std::vector<Ent> entities;
    for (auto row : stmtNum_entities) {
        for (auto entity : row.second) {
            stmtNums.push_back(row.first);
            entities.push_back(entity);
        }
    }
    return { stmtNums, entities };
}

std::pair<std::vector<std::string>, std::vector<std::string>> UsesStorage::getAllProcEntPairs() {
    std::vector<ProcName> procs;
    std::vector<Ent> entities;
    for (auto row : procName_entities) {
        for (auto entity : row.second) {
            procs.push_back(row.first);
            entities.push_back(entity);
        }
    }
    return { procs, entities };
}