#include "ParentTStorage.h"

void ParentTStorage::write(StmtNum parent, std::unordered_set<StmtNum> children) {
    leftToRightMap[parent].insert(children.begin(), children.end());
    for (StmtNum child: children) {
        rightToLeftMap[child].insert(parent);
    }
}
