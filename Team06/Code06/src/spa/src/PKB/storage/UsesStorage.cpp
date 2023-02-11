#include "UsesStorage.h"

void UsesStorage::writeUsesProcedure(ProcedureName name, std::vector<Ent> entities) {
    procName_ent[name].insert(entities.begin(), entities.end());
    return;
}

void UsesStorage::writeUsesStmtnum(StmtNum num, std::vector<Ent> entities) {
    stmtNum_ent[num].insert(entities.begin(), entities.end());
    return;
}

std::unordered_set<Ent> UsesStorage::getUsesProcedure(ProcedureName name) {
    // source code does not even contain this procedure
    if (procName_ent.find(name) == procName_ent.end()) {
        std::unordered_set<Ent> emptySet;
        return emptySet;
    }
    return procName_ent[name];
}
std::unordered_set<Ent> UsesStorage::getUsesStmtnum(StmtNum num) {
    // source code does not even contain this statement number
    if (stmtNum_ent.find(num) == stmtNum_ent.end()) {
        std::unordered_set<Ent> emptySet;
        return emptySet;
    }
    return stmtNum_ent[num];
}