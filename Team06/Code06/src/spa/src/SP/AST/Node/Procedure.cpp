#include "Procedure.h"

Procedure::Procedure() {

}

Procedure::Procedure(std::string procedureName) {
	this->procedureName = procedureName;
};

void Procedure::accept(ASTVisitor *visitor) const {
	visitor->visitProcedure(this);
};