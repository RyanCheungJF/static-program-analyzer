#pragma once

#include "Expression.h"
#include "Statement.h"

class AssignStatement : public Statement {
public:
    Ent varName;
    std::unique_ptr<Expression> expr;

    AssignStatement();
    AssignStatement(StmtNum stmtNum, Ent varName, std::unique_ptr<Expression> expr);

    void accept(ASTVisitor* visitor) override;
};