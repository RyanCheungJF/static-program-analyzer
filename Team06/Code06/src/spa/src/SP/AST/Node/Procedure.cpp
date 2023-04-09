#include "Procedure.h"

Procedure::Procedure() : procedureName(), statementList() {}

Procedure::Procedure(ProcName procedureName, std::unique_ptr<StatementList> statementList)
    : procedureName(std::move(procedureName)), statementList(std::move(statementList)) {}

std::vector<std::unique_ptr<Statement>>& Procedure::getStatements() const {
    return statementList->statements;
}

Statement* Procedure::getLastStatement() const {
    return statementList->statements.back().get();
}

StmtNum Procedure::getLastStatementNumber() const {
    return statementList->statements.back()->statementNumber;
}

StmtNum Procedure::getFirstStatementNumber() const {
    return statementList->statements.front()->statementNumber;
}

void Procedure::accept(ASTVisitor* visitor) {
    visitor->visitProcedure(this);
}
