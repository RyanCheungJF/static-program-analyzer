#include "ModifiesStorage.h"

//void ModifiesStorage::write(StmtNum num, Ent ent) {
//    stmtNum_entities[num].insert(ent);
//    entities_stmtNum[ent].insert(num);
//    return;
//}

void ModifiesStorage::writeModifiesS(StmtNum num, std::vector<Ent> entities) {
    stmtNum_entities[num].insert(entities.begin(), entities.end());
    for (Ent e: entities) {
        entities_stmtNum[e].insert(num);
    }
}

void ModifiesStorage::writeModifiesP(ProcName name, std::vector<Ent> entities) {
    procName_entities[name].insert(entities.begin(), entities.end());
    for (Ent e: entities) {
        entities_procName[e].insert(name);
    }
}

std::unordered_set<Ent> ModifiesStorage::getModifiesS(StmtNum num) {
    if (stmtNum_entities.find(num) == stmtNum_entities.end()) {
        std::unordered_set<Ent> emptySet;
        return emptySet;
    }
    return stmtNum_entities[num];
}

std::unordered_set<Ent> ModifiesStorage::getModifiesP(ProcName name) {
    if (procName_entities.find(name) == procName_entities.end()) {
        std::unordered_set<Ent> emptySet;
        return emptySet;
    }
    return procName_entities[name];
}