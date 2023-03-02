#pragma once

#include "ConditionalExpression.h"
#include "StatementList.h"

class WhileStatement : public Statement {
public:
    std::unique_ptr<ConditionalExpression> condExpr;
    std::unique_ptr<StatementList> stmtList;

    WhileStatement();
    WhileStatement(StmtNum stmtNum, std::unique_ptr<ConditionalExpression> condExpr,
                   std::unique_ptr<StatementList> stmtList);

    std::vector<std::unique_ptr<Statement>>& getStatements();
    StmtNum getFirstStmtNumForList();

    void accept(ASTVisitor* visitor) override;
};