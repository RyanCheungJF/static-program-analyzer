#include "ProcedureStorage.h"

void ProcedureStorage::writeProcedure(Proc p, std::vector<StmtNum> lines) {
    for (StmtNum line: lines) {
        proc_stmtNum[p].insert(line);
    }
    return;
}

void ProcedureStorage::writeProcedure(Proc p, StmtNum line) {
    proc_stmtNum[p].insert(line);
}

bool ProcedureStorage::checkProcedure(Proc p, StmtNum num) {

    // source code does not even contain this type of statement
    if (proc_stmtNum.find(p) == proc_stmtNum.end()) {
        return false;
    }
    return proc_stmtNum[p].find(num) != proc_stmtNum[p].end();
}

std::unordered_set<StmtNum> ProcedureStorage::getProcedureStatementNumbers(Proc p) {
    if (proc_stmtNum.find(p) == proc_stmtNum.end()) {
        std::unordered_set<StmtNum> emptySet;
        return emptySet;
    }
    return proc_stmtNum.at(p);
}
