#pragma once

#include "../../../src/utils/AppConstants.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class CallsStorage {
public:

  virtual void writeCallP(ProcName caller, std::unordered_set<ProcName> callees);
  virtual std::unordered_set<ProcName> getCallees(ProcName caller);
  virtual std::unordered_set<ProcName> getCallers(ProcName callee);

private:
    /*
     * Structure of table example below
     *  caller | callees
     *  "proc2" | {"proc3", "proc4"}
     *  "proc3" | {"proc4"}
     */
  std::unordered_map<ProcName , std::unordered_set<ProcName>> caller_callees;

    /*
     * Structure of table example below
     *  callee | callers
     *  "proc3" | {"proc2"}
     *  "proc4" | {"proc2", "proc3"}
     */
  std::unordered_map<ProcName , std::unordered_set<ProcName>> callee_callers;
};
