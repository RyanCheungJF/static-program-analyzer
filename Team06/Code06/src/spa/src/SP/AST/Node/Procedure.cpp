#include "Procedure.h"

Procedure::Procedure(StatementList statementList) {
	this->statementList = statementList;
};

void Procedure::accept(ASTVisitor visitor) {
	visitor->visitProcedure(this);
};