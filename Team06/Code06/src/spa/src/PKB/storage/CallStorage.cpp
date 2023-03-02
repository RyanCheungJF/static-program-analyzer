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
    std::pair<StmtNum, ProcName> emptyPair = std::make_pair<StmtNum, ProcName>(-1, "INVALID");
    return emptyPair;
  }
  return std::make_pair(s, callLine_callee[s]);
}


void CallStorage::writeCallP(ProcName caller, std::unordered_set<ProcName> callees) {
    caller_callees[caller].insert(callees.begin(), callees.end());
    for (ProcName callee : callees) {
        callee_callers[callee].insert(caller);
    }
}

std::unordered_set<ProcName> CallStorage::getCallees(ProcName caller) {
    if (caller_callees.find(caller) == caller_callees.end()) {
        std::unordered_set<ProcName> emptySet;
        return emptySet;
    }
    return caller_callees[caller];
}

std::unordered_set<ProcName> CallStorage::getCallers(ProcName callee) {
    if (callee_callers.find(callee) == callee_callers.end()) {
        std::unordered_set<ProcName> emptySet;
        return emptySet;
    }
    return callee_callers[callee];
}
