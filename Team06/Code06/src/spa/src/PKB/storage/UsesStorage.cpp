#include "UsesStorage.h"

void UsesStorage::writeUsesProcedure(ProcedureName name, std::vector<Ent> entities) {
    procName_ent[name].insert(entities.begin(), entities.end());
}

void UsesStorage::writeUsesStmtnum(StmtNum num, std::vector<Ent> entities) {
    stmtNum_ent[num].insert(entities.begin(), entities.end());
}

std::unordered_set<Ent> UsesStorage::getUsesProcedureEntities(ProcedureName name) {
    return procName_ent[name];
}
std::unordered_set<Ent> UsesStorage::getUsesStmtnumEntities(StmtNum num) {
    return stmtNum_ent[num];
}