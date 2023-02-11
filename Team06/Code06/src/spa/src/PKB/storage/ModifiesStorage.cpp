#include "ModifiesStorage.h"

void ModifiesStorage::writeModifiesProcedure(ProcedureName name, std::vector<Ent> entities) {
    procName_ent[name].insert(entities.begin(), entities.end());
}

void ModifiesStorage::writeModifiesStmtnum(StmtNum num, std::vector<Ent> entities) {
    stmtNum_ent[num].insert(entities.begin(), entities.end());
}

std::unordered_set<Ent> ModifiesStorage::getModifiesProcedureEntities(ProcedureName name) {
    return procName_ent[name];
}
std::unordered_set<Ent> ModifiesStorage::getModifiesStmtnumEntities(StmtNum num) {
    return stmtNum_ent[num];
}