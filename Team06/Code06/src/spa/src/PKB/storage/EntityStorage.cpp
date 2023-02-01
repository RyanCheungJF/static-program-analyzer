#include "EntityStorage.h"

void EntityStorage::writeEntity(Ent e, std::vector<StmtNum> lines) {
    for (StmtNum line: lines) {
        ent_stmtNum[e].insert(line);
    }
    return;
}

bool EntityStorage::checkEntity(Ent e, StmtNum num) {

    // source code does not even contain this type of statement
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
