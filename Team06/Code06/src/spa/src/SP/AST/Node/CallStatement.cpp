#include "CallStatement.h"

CallStatement::CallStatement() : procName() {}

CallStatement::CallStatement(StmtNum stmtNum, ProcName procName) {
    this->statementNumber = stmtNum;
    this->procName = std::move(procName);
}

void CallStatement::accept(ASTVisitor* visitor) {
    visitor->visitCallStatement(this);
}