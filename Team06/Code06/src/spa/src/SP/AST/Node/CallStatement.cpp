#include "CallStatement.h"

CallStatement::CallStatement() {}

CallStatement::CallStatement(StmtNum stmtNum, ProcName procName, ProcName parentProc) {
  this->statementNumber = stmtNum;
  this->procName = procName;
  this->parentProcedure = parentProc;
}

void CallStatement::accept(ASTVisitor *visitor) {
  visitor->visitCallStatement(this);
}