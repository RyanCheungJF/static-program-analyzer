#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>
#include "../../../src/utils/AppConstants.h"

class CallStorage {
public:
    virtual void writeCall(StmtNum callLine, ProcName callee);
    virtual std::vector<std::pair<StmtNum, ProcName>> getCallStatements();
    virtual std::pair<StmtNum, ProcName> getCallStmt(StmtNum s);
    
private:
    /*
     * Structure of table example below
     *  call | callee
     *  11 | "proc2"
     *  28 | "proc3"
     */
    std::unordered_map<StmtNum, std::string> callLine_callee;
};
