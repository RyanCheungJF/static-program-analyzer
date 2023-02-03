#include "Procedure.h"

void Procedure::accept(ASTVisitor* visitor) const {
	visitor->visitProcedure(this);
};