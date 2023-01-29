#pragma once

#include <vector>
#include "Constant.h"
#include "Expression.h"
#include "Variable.h"

class RelationalExpression : public Expression {
public:
	//std::vector<Constant> constantsList;
	//std::vector<Variable> variablesList;

	RelationalExpression();

	//RelationalExpression(std::vector<Constant> constantsList, std::vector<Variable> variablesList);

	void accept(ASTVisitor* visitor) const override;
};