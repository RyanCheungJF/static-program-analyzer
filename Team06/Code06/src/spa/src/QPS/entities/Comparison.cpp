#include "Comparison.h"
#include "QPS/syntaxValidator/CompSyntaxValidator.h"

Comparison::Comparison()
{
}

Comparison::Comparison(ComparisonOperator o, Parameter leftP, Parameter rightP)
{
	op = o;
	leftParam = leftP;
	rightParam = rightP;
}

Parameter Comparison::getLeftParam()
{
	return leftParam;
}

Parameter Comparison::getRightParam()
{
	return rightParam;
}

ComparisonOperator Comparison::getOperator()
{
	return op;
}

vector<Parameter*> Comparison::getAllUncheckedSynonyms()
{
	vector<Parameter*> synonyms;
	if (leftParam.isUncheckedSynonym()) {
		synonyms.push_back(&leftParam);
	}
	if (rightParam.isUncheckedSynonym()) {
		synonyms.push_back(&rightParam);
	}
	return synonyms;
}

Comparison Comparison::makeComparison(string o, Parameter leftP, Parameter rightP)
{
	CompSyntaxValidator syntaxVal;
	Comparison comp =  Comparison(stringToOp(o), leftP, rightP);
	syntaxVal.validate(comp);
	return comp;
}

ComparisonOperator Comparison::stringToOp(string s)
{
	auto iter = stringToOpMap.find(s);
	if (iter == stringToOpMap.end()) {
		throw SyntaxException();
	}
	return iter->second;
}

const unordered_map<string, ComparisonOperator> Comparison::stringToOpMap = {
	{AppConstants::OP_EQUALS, ComparisonOperator::EQUALS},
};