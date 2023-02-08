#include "Procedure.h"

Procedure::Procedure() {}

Procedure::Procedure(std::string procedureName, std::unique_ptr<StatementList> statementList) {
	this->procedureName = procedureName;
	this->statementList = std::move(statementList);
}

void Procedure::accept(ASTVisitor* visitor) {
	visitor->visitProcedure(this);
}
