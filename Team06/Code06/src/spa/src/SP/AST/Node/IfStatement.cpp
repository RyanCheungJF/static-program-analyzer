#include "IfStatement.h"

IfStatement::IfStatement(){};

IfStatement::IfStatement(StmtNum stmtNum, std::unique_ptr<ConditionalExpression> condExpr,
                         std::unique_ptr<StatementList> thenStmtList, std::unique_ptr<StatementList> elseStmtList) {
    this->statementNumber = stmtNum;
    this->condExpr = std::move(condExpr);
    this->thenStmtList = std::move(thenStmtList);
    this->elseStmtList = std::move(elseStmtList);
}

std::vector<std::unique_ptr<Statement>>& IfStatement::getThenStatements() {
    return thenStmtList->statements;
}

std::vector<std::unique_ptr<Statement>>& IfStatement::getElseStatements() {
    return elseStmtList->statements;
}

void IfStatement::accept(ASTVisitor* visitor) {
    visitor->visitIfStatement(this);
}