#include "ProcedureStorage.h"

void ProcedureStorage::writeProcedure(ProcName p, std::unordered_set<StmtNum>& lines) {
    for (StmtNum line : lines) {
        proc_stmtNum[p].insert(line);
        stmtNum_proc[line] = p;
    }
    procedures.insert(p);
}

void ProcedureStorage::writeProcedure(ProcName p, StmtNum num) {
    proc_stmtNum[p].insert(num);
    stmtNum_proc[num] = p;
    procedures.insert(p);

}

bool ProcedureStorage::checkProcedure(ProcName p, StmtNum num) {
    if (proc_stmtNum.find(p) == proc_stmtNum.end()) {
        return false;
    }
    return proc_stmtNum[p].find(num) != proc_stmtNum[p].end();
}

std::unordered_set<StmtNum>& ProcedureStorage::getProcedureStatementNumbers(ProcName p) {
    if (proc_stmtNum.find(p) == proc_stmtNum.end()) {
        return emptyStmtNums;
    }
    return proc_stmtNum[p];
}

std::unordered_set<ProcName>& ProcedureStorage::getProcNames() {
    return procedures;
}

ProcName ProcedureStorage::getProcedure(StmtNum num) {
    if (stmtNum_proc.find(num) == stmtNum_proc.end()) {
        return AppConstants::PROCEDURE_DOES_NOT_EXIST;
    }
    return stmtNum_proc[num];
}
