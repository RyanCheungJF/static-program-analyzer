#include "CallStorage.h"

void CallStorage::writeCallS(StmtNum callLine, ProcName callee) {
    callLine_callee[callLine] = callee;
}


std::pair<StmtNum, ProcName> CallStorage::getCallStmt(StmtNum s) {

    if (callLine_callee.find(s) == callLine_callee.end()) {
        return {AppConstants::NOT_USED_FIELD, AppConstants::PROCEDURE_DOES_NOT_EXIST};
    }
    return std::make_pair(s, callLine_callee[s]);
}
