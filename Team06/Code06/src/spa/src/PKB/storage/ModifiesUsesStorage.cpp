#include "ModifiesUsesStorage.h"

std::unordered_set<StmtNum>* ModifiesUsesStorage::getStmtNums(Ent ent) {
    return this->getLeftItems(ent);
}

std::unordered_set<Ent>* ModifiesUsesStorage::getProcs(Ent ent) {
    return RelationshipStorage<Ent, Ent>::getLeftItems(ent);
}

std::unordered_set<Ent>* ModifiesUsesStorage::getAllProcs() {
    return RelationshipStorage<Ent, Ent>::getAllLeftItems();
}

std::unordered_set<StmtNum>* ModifiesUsesStorage::getAllStmtNums() {
    return RelationshipStorage<StmtNum, Ent>::getAllLeftItems();
}