#include "Variable.h"

Variable::Variable() {}

Variable::Variable(std::string name) {
	this->name = name;
}

void Variable::accept(ASTVisitor* visitor) {
	visitor->visitExpression(this);
};
