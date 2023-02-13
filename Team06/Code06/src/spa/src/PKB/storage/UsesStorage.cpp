#include "UsesStorage.h"

//void UsesStorage::write(StmtNum num, Ent ent) {
//    stmtNum_entities[num].insert(ent);
//    entities_stmtNum[ent].insert(num);
//    return;
//}

void UsesStorage::writeUsesS(StmtNum num, std::vector<Ent> entities) {
    stmtNum_entities[num].insert(entities.begin(), entities.end());
    for (Ent e: entities) {
        entities_stmtNum[e].insert(num);
    }
}

void UsesStorage::writeUsesP(ProcName name, std::vector<Ent> entities) {
    procName_entities[name].insert(entities.begin(), entities.end());
    for (Ent e: entities) {
        entities_procName[e].insert(name);
    }
}

std::unordered_set<Ent> UsesStorage::getUsesS(StmtNum num) {
    if (stmtNum_entities.find(num) == stmtNum_entities.end()) {
        std::unordered_set<Ent> emptySet;
        return emptySet;
    }
    return stmtNum_entities[num];
}

std::unordered_set<Ent> UsesStorage::getUsesP(ProcName name) {
    if (procName_entities.find(name) == procName_entities.end()) {
        std::unordered_set<Ent> emptySet;
        return emptySet;
    }
    return procName_entities[name];
}