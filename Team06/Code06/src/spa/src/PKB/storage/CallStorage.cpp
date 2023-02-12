#include "CallStorage.h"

void CallStorage::writeCall(Stmt caller, Stmt callee) {
    caller_callee[caller].insert(callee);
}
