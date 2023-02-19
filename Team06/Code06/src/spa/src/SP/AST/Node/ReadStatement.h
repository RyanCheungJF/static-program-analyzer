#pragma once

#include "Statement.h"

class ReadStatement : public Statement {
public:
	Ent varName;

	ReadStatement();
	ReadStatement(StmtNum stmtNum, Ent varName);

	void accept(ASTVisitor* visitor) override;
};