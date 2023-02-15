#include "EntityStorage.h"

void EntityStorage::writeEntity(StmtNum num, std::unordered_set<Ent> entities) {
    stmtNum_ent[num].insert(entities.begin(), entities.end());
    for (Ent e : entities) {
        ent_stmtNum[e].insert(num);
    }

    for (Ent e : entities) {
        entNames.insert(e);
    }
}

bool EntityStorage::checkEntity(Ent e, StmtNum num) {

    // source code does not even contain this entity
    if (ent_stmtNum.find(e) == ent_stmtNum.end()) {
        return false;
    }
    return ent_stmtNum[e].find(num) != ent_stmtNum[e].end();
}

std::unordered_set<StmtNum> EntityStorage::getEntityStmtNums(Ent e) {
    if (ent_stmtNum.find(e) == ent_stmtNum.end()) {
        std::unordered_set<StmtNum> emptySet;
        return emptySet;
    }
    return ent_stmtNum.at(e);
}

std::unordered_set<std::string> EntityStorage::getEntNames() {
    return entNames;
}
