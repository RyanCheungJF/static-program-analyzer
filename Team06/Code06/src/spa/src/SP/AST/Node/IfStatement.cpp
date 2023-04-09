#include "IfStatement.h"

IfStatement::IfStatement() : condExpr(), thenStmtList(), elseStmtList(){};

IfStatement::IfStatement(StmtNum stmtNum, std::unique_ptr<ConditionalExpression> condExpr,
                         std::unique_ptr<StatementList> thenStmtList, std::unique_ptr<StatementList> elseStmtList) {
    this->statementNumber = stmtNum;
    this->condExpr = std::move(condExpr);
    this->thenStmtList = std::move(thenStmtList);
    this->elseStmtList = std::move(elseStmtList);
}

std::vector<std::unique_ptr<Statement>>& IfStatement::getThenStatements() const {
    return thenStmtList->statements;
}

std::vector<std::unique_ptr<Statement>>& IfStatement::getElseStatements() const {
    return elseStmtList->statements;
}

StmtNum IfStatement::getFirstStmtNumForThen() const {
    return thenStmtList->statements[0]->statementNumber;
}

StmtNum IfStatement::getFirstStmtNumForElse() const {
    return elseStmtList->statements[0]->statementNumber;
}

void IfStatement::accept(ASTVisitor* visitor) {
    visitor->visitIfStatement(this);
}