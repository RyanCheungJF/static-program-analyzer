#include "Statement.h"

Statement::Statement() {

}

void Statement::accept(ASTVisitor* visitor) const {
	visitor->visitStatement(this);
};
