#include "ProcedureStorage.h"

void ProcedureStorage::writeProcedure(ProcName p, std::unordered_set<StmtNum> lines) {
  for (StmtNum line : lines) {
    proc_stmtNum[p].insert(line);
  }
  procedures.insert(p);
}

bool ProcedureStorage::checkProcedure(ProcName p, StmtNum num) {

  // source code does not even contain this type of statement
  if (proc_stmtNum.find(p) == proc_stmtNum.end()) {
    return false;
  }
  return proc_stmtNum[p].find(num) != proc_stmtNum[p].end();
}

std::unordered_set<StmtNum> ProcedureStorage::getProcedureStatementNumbers(ProcName p) {
  if (proc_stmtNum.find(p) == proc_stmtNum.end()) {
    std::unordered_set<StmtNum> emptySet;
    return emptySet;
  }
  return proc_stmtNum.at(p);
}

std::unordered_set<ProcName> ProcedureStorage::getProcNames() {
  return procedures;
}
