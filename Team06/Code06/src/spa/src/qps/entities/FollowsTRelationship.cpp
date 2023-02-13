#include "FollowsTRelationship.h"

bool FollowsTRelationship::validateParams(vector<Parameter> ps)
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

FollowsTRelationship::FollowsTRelationship(vector<Parameter> ps)
{
	if (!validateParams(ps)) {
		throw - 1;
	}
	type = RelationshipType::FOLLOWST;
	params = ps;
}
