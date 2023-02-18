#pragma once

#include "Statement.h"
#include "Expression.h"

class AssignStatement : public Statement {
public:
	Ent varName;
	std::unique_ptr<Expression> expr;

	AssignStatement();
	AssignStatement(Ent varName, std::unique_ptr<Expression> expr);
	AssignStatement(StmtNum stmtNum, Ent varName, std::unique_ptr<Expression> expr);

	void accept(ASTVisitor* visitor) override;
};