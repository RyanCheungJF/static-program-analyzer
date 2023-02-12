#include "CallStorage.h"

void CallStorage::writeCall(StmtNum callLine, ProcName callee) {
    callLine_callee[callLine]= callee;
    return;
}

std::vector<std::pair<StmtNum, ProcName>> CallStorage::getCallStatements() {
    std::vector<std::pair<StmtNum, ProcName>> res;
    for (auto i : callLine_callee) {
        res.push_back(std::make_pair(i.first, i.second));
    }
    return res;
}
