#include "FollowsRelationship.h"

bool FollowsRelationship::validateParams(vector<Parameter> ps)
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

FollowsRelationship::FollowsRelationship(vector<Parameter> ps)
{
	if (!FollowsRelationship::validateParams(ps)) {
		throw -1;
	}
	type = RelationshipType::FOLLOWS;
	params = ps;
}
