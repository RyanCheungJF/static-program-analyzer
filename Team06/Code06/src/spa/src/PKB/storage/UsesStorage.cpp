#include "UsesStorage.h"

void UsesStorage::write(StmtNum num, Ent ent) {
    stmtNum_entities[num].insert(ent);
    entities_stmtNum[ent].insert(num);
    return;
}

bool UsesStorage::exists(StmtNum num, Ent ent) {
    if (stmtNum_entities.find(num) == stmtNum_entities.end()) {
        return false;
    }
    return stmtNum_entities[num].find(ent) != stmtNum_entities[num].end();
}

std::unordered_set<Ent> UsesStorage::getLeftWildcard(StmtNum followee) {
    std::unordered_set<Ent> entities;

    for (auto i : entities_stmtNum) {
        entities.insert(i.first);
    }
    return entities;
}