#include "ModifiesRelationship.h"

bool ModifiesRelationship::validateParams(vector<Parameter>& ps)
{
	if (ps.size() != 2) {
		return false;
	}
	if (!Parameter::isStatementRef(ps[0])) {
		return false;
	}
	if (!Parameter::isEntityRef(ps[1])) {
		return false;
	}
	return true;
}

ModifiesRelationship::ModifiesRelationship(vector<Parameter>& ps)
{
	if (!validateParams(ps)) {
		throw - 1;
	}
	type = RelationshipType::MODIFIES;
	params = ps;
}