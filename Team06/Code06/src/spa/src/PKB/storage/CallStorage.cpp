#include "CallStorage.h"

void CallStorage::writeCallS(StmtNum callLine, ProcName callee) {
    callLine_callee[callLine] = callee;
}

std::vector<std::pair<StmtNum, ProcName>> CallStorage::getCallStatements() {
    std::vector<std::pair<StmtNum, ProcName>> res;
    for (auto i : callLine_callee) {
        res.push_back(std::make_pair(i.first, i.second));
    }
    return res;
}

std::pair<StmtNum, ProcName> CallStorage::getCallStmt(StmtNum s) {

    // SHOULD NOT HIT.
    if (callLine_callee.find(s) == callLine_callee.end()) {
        return {AppConstants::NOT_USED_FIELD, AppConstants::PROCEDURE_DOES_NOT_EXIST};
    }
    return std::make_pair(s, callLine_callee[s]);
}
