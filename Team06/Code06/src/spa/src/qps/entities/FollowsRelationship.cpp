#include "FollowsRelationship.h"

bool FollowsRelationship::validateSyntax(vector<Parameter> ps)
{
	if (ps.size() != 2) {
		return false;
	}
	if (!Parameter::isSyntacticStatementRef(ps[0])) {
		return false;
	}
	if (!Parameter::isSyntacticStatementRef(ps[1])) {
		return false;
	}
	return true;
}

FollowsRelationship::FollowsRelationship(vector<Parameter> ps)
{
	if (!validateSyntax(ps)) {
		throw -1;
	}
	type = RelationshipType::FOLLOWS;
	params = ps;
}

bool FollowsRelationship::validateParams()
{
	if (!Parameter::isStatementRef(params[0])) {
		return false;
	}
	if (!Parameter::isStatementRef(params[1])) {
		return false;
	}
	return true;
}
