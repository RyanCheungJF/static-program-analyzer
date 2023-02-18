#pragma once

#include "Statement.h"

class PrintStatement : public Statement {
public:
	Ent varName;

	PrintStatement();
	PrintStatement(Ent varName);
	PrintStatement(StmtNum stmtNum, Ent varName);

	void accept(ASTVisitor* visitor) override;
};