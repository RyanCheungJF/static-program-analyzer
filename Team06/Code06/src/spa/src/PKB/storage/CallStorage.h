#pragma once

#include "../../../src/utils/AppConstants.h"

class CallStorage {
public:
    virtual void writeCallS(StmtNum callLine, ProcName callee);
    virtual std::pair<StmtNum, ProcName> getCallStmt(StmtNum s);

private:
    /*
     * Structure of table example below
     *  call | callee
     *  11 | "proc2"
     *  28 | "proc3"
     */
    std::unordered_map<StmtNum, ProcName> callLine_callee;
};
