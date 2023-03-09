#include "WhileStatement.h"

WhileStatement::WhileStatement() : condExpr(), stmtList(){};

WhileStatement::WhileStatement(StmtNum stmtNum, std::unique_ptr<ConditionalExpression> condExpr,
                               std::unique_ptr<StatementList> stmtList) {
    this->statementNumber = stmtNum;
    this->condExpr = std::move(condExpr);
    this->stmtList = std::move(stmtList);
}

std::vector<std::unique_ptr<Statement>>& WhileStatement::getStatements() {
    return stmtList->statements;
}

StmtNum WhileStatement::getFirstStmtNumForList() {
    return stmtList->statements[0]->statementNumber;
}

void WhileStatement::accept(ASTVisitor* visitor) {
    visitor->visitWhileStatement(this);
}