#include "Variable.h"

Variable::Variable() {}

Variable::Variable(std::string variableName, std::string value) {
	this->variableName = variableName;
	this->value = value;
}

void Variable::accept(ASTVisitor *visitor) const {
	visitor->visitVariable(this);
};
