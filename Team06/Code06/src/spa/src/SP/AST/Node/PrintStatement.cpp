#include "PrintStatement.h"

void PrintStatement::accept(ASTVisitor* visitor) const {
	visitor->visitPrintStatement(this);
}