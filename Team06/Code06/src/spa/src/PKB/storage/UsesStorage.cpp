#include "UsesStorage.h"

void UsesStorage::writeUsesProcedure(ProcedureName name, std::vector<Ent> entities) {
    procName_ent[name].insert(entities.begin(), entities.end());
}

void UsesStorage::writeUsesStmtnum(StmtNum num, std::vector<Ent> entities) {

}
