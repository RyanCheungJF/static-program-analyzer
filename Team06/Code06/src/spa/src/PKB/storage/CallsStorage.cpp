#include "CallsStorage.h"

void CallsStorage::writeCallP(ProcName caller, std::unordered_set<ProcName> callees) {
    caller_callees[caller].insert(callees.begin(), callees.end());
    for (ProcName callee : callees) {
        callee_callers[callee].insert(caller);
    }
}

std::unordered_set<ProcName> CallsStorage::getCallees(ProcName caller) {
    if (caller_callees.find(caller) == caller_callees.end()) {
        std::unordered_set<ProcName> emptySet;
        return emptySet;
    }
    return caller_callees[caller];
}

std::unordered_set<ProcName> CallsStorage::getCallers(ProcName callee) {
    if (callee_callers.find(callee) == callee_callers.end()) {
        std::unordered_set<ProcName> emptySet;
        return emptySet;
    }
    return callee_callers[callee];
}
