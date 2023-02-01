#include "Relationship.h"

Relationship Relationship::makeRelationship(string type, vector<Parameter> params)
{
	RelationshipType rType = stringToType(type);
	switch (rType) {
	case RelationshipType::FOLLOWS:
		return FollowsRelationship(params);
	}
	throw - 1;
}

RelationshipType Relationship::stringToType(string s)
{
	auto iter = Relationship::stringToTypeMap.find(s);
	if (iter == stringToTypeMap.end()) {
		return RelationshipType::UNKNOWN;
	}
	return iter->second;
}

const unordered_map<string, RelationshipType> Relationship::stringToTypeMap = {
	{"Follows" , RelationshipType::FOLLOWS}
}
