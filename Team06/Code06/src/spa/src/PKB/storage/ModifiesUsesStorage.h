#pragma once

#include "../../../src/utils/AppConstants.h"
#include "RelationshipStorage.h"

class ModifiesUsesStorage : public RelationshipStorage<StmtNum, Ent>, public RelationshipStorage<Ent, Ent> {

public:
    using RelationshipStorage<StmtNum, Ent>::write;
    using RelationshipStorage<Ent, Ent>::write;
    using RelationshipStorage<StmtNum, Ent>::exists;
    using RelationshipStorage<Ent, Ent>::exists;
    using RelationshipStorage<StmtNum, Ent>::getLeftItems;
    using RelationshipStorage<StmtNum, Ent>::getRightItems;
    using RelationshipStorage<Ent, Ent>::getRightItems;
    std::unordered_set<StmtNum>& getStmtNums(Ent ent);
    std::unordered_set<Ent>& getProcs(Ent ent);
    std::unordered_set<Ent>& getAllProcs();
    std::unordered_set<StmtNum>& getAllStmtNums();
};