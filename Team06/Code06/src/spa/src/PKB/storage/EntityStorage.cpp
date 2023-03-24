#include "EntityStorage.h"

template <typename T> void EntityStorage<T>::writeEntity(StmtNum num, std::unordered_set<T>& entities) {
    stmtNum_ent[num].insert(entities.begin(), entities.end());
    for (auto e : entities) {
        ent_stmtNum[e].insert(num);
        entNames.insert(e);
    }
}

template <typename T> bool EntityStorage<T>::checkEntity(T e, StmtNum num) {
    if (ent_stmtNum.find(e) == ent_stmtNum.end()) {
        return false;
    }
    return ent_stmtNum[e].find(num) != ent_stmtNum[e].end();
}

template <typename T> std::unordered_set<StmtNum> EntityStorage<T>::getEntityStmtNums(T e) {
    if (ent_stmtNum.find(e) == ent_stmtNum.end()) {
        std::unordered_set<StmtNum> emptySet;
        return emptySet;
    }
    return ent_stmtNum.at(e);
}

template <typename T> std::unordered_set<T> EntityStorage<T>::getEntities(StmtNum num) {
    if (stmtNum_ent.find(num) == stmtNum_ent.end()) {
        std::unordered_set<T> emptySet;
        return emptySet;
    }
    return stmtNum_ent.at(num);
}

template <typename T> std::unordered_set<T> EntityStorage<T>::getEntNames() {
    return entNames;
}

template class EntityStorage<Ent>;
template class EntityStorage<Const>;