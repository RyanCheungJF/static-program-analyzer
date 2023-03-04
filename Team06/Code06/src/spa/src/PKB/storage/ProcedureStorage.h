#pragma once

#include "../../../src/utils/AppConstants.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class ProcedureStorage {
public:
    void writeProcedure(ProcName p, std::unordered_set<StmtNum> lines);
    bool checkProcedure(ProcName p, StmtNum num);
    std::unordered_set<StmtNum> getProcedureStatementNumbers(ProcName p);
    ProcName getProcedure(StmtNum num);
    std::unordered_set<ProcName> getProcNames();

private:
    /*
     * Structure of table
     *  ProcName | statementNumbers
     *  "proc1" | [1, 2, 3, ...]
     *  "proc2" | [7, 9, ...]
     */
    std::unordered_map<ProcName, std::unordered_set<StmtNum>> proc_stmtNum;
    std::unordered_map<StmtNum, ProcName> stmtNum_proc;
    std::unordered_set<ProcName> procedures;
};
