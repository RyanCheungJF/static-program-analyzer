#include "UsesRelationship.h"

bool UsesRelationship::validateParams(vector<Parameter>& ps)
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

UsesRelationship::UsesRelationship(vector<Parameter>& ps)
{
	if (!validateParams(ps)) {
		throw - 1;
	}
	type = RelationshipType::USES;
	params = ps;
}

