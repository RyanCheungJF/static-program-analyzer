#include "Relationship.h"
#include "FollowsRelationship.h"

Relationship Relationship::makeRelationship(string type, vector<Parameter> params)
{
	RelationshipType rType = stringToType(type);
	switch (rType) {
	case RelationshipType::FOLLOWS:
		return FollowsRelationship(params);
	}
	throw - 1;
}

Relationship::Relationship(const Relationship& r)
{
	type = r.type;
	params = r.params;
}

Relationship::Relationship()
{
	type = RelationshipType::UNKNOWN;
	params;
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
};
