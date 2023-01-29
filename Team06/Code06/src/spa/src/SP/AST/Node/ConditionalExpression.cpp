#include "ConditionalExpression.h"

//ConditionalExpression::ConditionalExpression(std::vector<RelationalExpression> relationsList) {
//	this->relationsList = relationsList;
//};

ConditionalExpression::ConditionalExpression() {}

void ConditionalExpression::accept(ASTVisitor* visitor) const {
	visitor->visitConditionalExpression(this);
}
