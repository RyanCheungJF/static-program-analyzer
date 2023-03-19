#include "CompSyntaxValidator.h"

CompSyntaxValidator::CompSyntaxValidator()
{
}

bool CompSyntaxValidator::validate(Comparison& c)
{
	if (c.getLeftParam().getComparisonType() == ParameterType::UNKNOWN) {
		throw SyntaxException();
	}
	if (c.getRightParam().getComparisonType() == ParameterType::UNKNOWN) {
		throw SyntaxException();
	}
}
