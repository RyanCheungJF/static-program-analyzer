#include "ParentTRelationship.h"

bool ParentTRelationship::validateParams(vector<Parameter>& ps)
{
	if (ps.size() != 2) {
		return false;
	}
	if (!Parameter::isStatementRef(ps[0])) {
		return false;
	}
	if (!Parameter::isStatementRef(ps[1])) {
		return false;
	}
	return true;
}

ParentTRelationship::ParentTRelationship(vector<Parameter>& ps)
{
	if (!validateParams(ps)) {
		throw - 1;
	}
	type = RelationshipType::PARENTT;
	params = ps;
}