#include "RelationalExpression.h"

RelationalExpression::RelationalExpression(std::vector<Constant> constantsList, std::vector<Variable> variablesList) {
	this->constantsList = constantsList;
	this->variablesList = variablesList;
};