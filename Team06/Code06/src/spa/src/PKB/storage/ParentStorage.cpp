#include "ParentStorage.h"

void ParentStorage::write(StmtNum parent, StmtNum child) {
    leftToRightMap[parent].insert(child);
    rightToLeftMap[child].insert(parent);
}
