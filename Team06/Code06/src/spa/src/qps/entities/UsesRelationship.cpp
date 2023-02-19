#include "UsesRelationship.h"

bool UsesRelationship::validateSyntax(vector<Parameter>& ps)
{
	if (ps.size() != 2) {
		return false;
	}
	if (!Parameter::isSyntacticStatementRef(ps[0]) && !Parameter::isProcedure(ps[0])) {
		return false;
	}
	if (!Parameter::isSyntacticEntityRef(ps[1])) {
		return false;
	}
	return true;
}

UsesRelationship::UsesRelationship(vector<Parameter>& ps)
{
	if (!validateSyntax(ps)) {
		throw SyntaxException();
	}
	type = RelationshipType::USES;
	params = ps;
}

bool UsesRelationship::validateParams()
{
	if (params[0].getValue() == "_" ||
    (!Parameter::isStatementRef(params[0]) && !Parameter::isProcedure(params[0]))) {
		return false;
	}
	if (!Parameter::isEntityRef(params[1])) {
		return false;
	}
	return true;
}

