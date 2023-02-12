#include "ContainerStorage.h"

void ContainerStorage::writeContainerBlock(StmtNum num, StmtNum start, StmtNum end, std::vector<StmtNum> directly_nested_containerStmt) {
    stmtNum_range_nestedContainerStmts[num] = {{start, end}, directly_nested_containerStmt};
    return;
}

