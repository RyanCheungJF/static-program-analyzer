#include "Procedure.h"

Procedure::Procedure() : procedureName(), statementList() {}

Procedure::Procedure(ProcName procedureName, std::unique_ptr<StatementList> statementList)
    : procedureName(procedureName), statementList(std::move(statementList)) {}

std::vector<std::unique_ptr<Statement>>& Procedure::getStatements() {
    return statementList->statements;
}

Statement* Procedure::getLastStatement() {
    return statementList->statements.back().get();
}

StmtNum Procedure::getLastStatementNumber() {
    return statementList->statements.back()->statementNumber;
}

StmtNum Procedure::getFirstStatementNumber() {
    return statementList->statements.front()->statementNumber;
}

void Procedure::accept(ASTVisitor* visitor) {
    visitor->visitProcedure(this);
}
