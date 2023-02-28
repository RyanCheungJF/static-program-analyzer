#include "WhileStatement.h"

WhileStatement::WhileStatement(){};

WhileStatement::WhileStatement(StmtNum stmtNum,
                               std::unique_ptr<ConditionalExpression> condExpr,
                               std::unique_ptr<StatementList> stmtList) {
  this->statementNumber = stmtNum;
  this->condExpr = std::move(condExpr);
  this->stmtList = std::move(stmtList);
}

void WhileStatement::accept(ASTVisitor *visitor) {
  visitor->visitWhileStatement(this);
}