#include "ModifiesRelationship.h"

bool ModifiesRelationship::validateSyntax(vector<Parameter>& ps)
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

ModifiesRelationship::ModifiesRelationship(vector<Parameter>& ps)
{
	if (!validateSyntax(ps)) {
		throw - 1;
	}
	type = RelationshipType::MODIFIES;
	params = ps;
}

bool ModifiesRelationship::validateParams()
{
	if (!Parameter::isStatementRef(params[0]) && !Parameter::isProcedure(params[0])) {
		return false;
	}
	if (!Parameter::isEntityRef(params[1])) {
		return false;
	}
	return true;
}
