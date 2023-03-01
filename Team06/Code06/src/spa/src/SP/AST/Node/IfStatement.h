#pragma once

#include "ConditionalExpression.h"
#include "StatementList.h"

class IfStatement : public Statement {
public:
    std::unique_ptr<ConditionalExpression> condExpr;
    std::unique_ptr<StatementList> thenStmtList;
    std::unique_ptr<StatementList> elseStmtList;

    IfStatement();
    IfStatement(StmtNum stmtNum, std::unique_ptr<ConditionalExpression> condExpr,
                std::unique_ptr<StatementList> thenStmtList, std::unique_ptr<StatementList> elseStmtList);

    std::vector<std::unique_ptr<Statement>>& getThenStatements();
    std::vector<std::unique_ptr<Statement>>& getElseStatements();

    void accept(ASTVisitor* visitor) override;
};