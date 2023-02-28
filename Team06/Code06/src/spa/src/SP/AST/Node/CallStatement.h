#pragma once

#include "Statement.h"

class CallStatement : public Statement {
public:
  ProcName procName;
  ProcName parentProcedure;

  CallStatement();
  CallStatement(StmtNum stmtNum, ProcName procName, ProcName parentProc);

  void accept(ASTVisitor *visitor) override;
};