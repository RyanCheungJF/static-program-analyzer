#pragma once

#include "Statement.h"

class CallStatement : public Statement {
public:
	ProcName procName;

	CallStatement();
	CallStatement(ProcName procName);
	CallStatement(StmtNum stmtNum, ProcName procName);

	void accept(ASTVisitor* visitor) override;
};