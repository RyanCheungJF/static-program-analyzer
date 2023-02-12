#include "CallStorage.h"

//void CallStorage::writeCall(Stmt caller, Stmt callee) {
//    caller_callee[caller].insert(callee);
//}

void CallStorage::writeCall(StmtNum callLine, Stmt callee) {
    callLine_callee[callLine]= callee;
    return;
}
