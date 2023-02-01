#include "Variable.h"

Variable::Variable(std::string val) {
	name = val;
}

void Variable::accept(ASTVisitor* visitor) const {
	visitor->visitExpression(this);
};
