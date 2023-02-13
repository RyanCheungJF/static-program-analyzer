#include "Relationship.h"
#include "FollowsRelationship.h"
#include "FollowsTRelationship.h"
#include "ParentRelationship.h"
#include "ParentTRelationship.h"
#include "UsesRelationship.h"
#include "ModifiesRelationship.h"

Relationship Relationship::makeRelationship(string type, vector<Parameter> params)
{
	RelationshipType rType = stringToType(type);
	switch (rType) {
	case RelationshipType::FOLLOWS:
		return FollowsRelationship(params);
	case RelationshipType::FOLLOWST:
		return FollowsTRelationship(params);
	case RelationshipType::PARENT:
		return ParentRelationship(params);
	case RelationshipType::PARENTT:
		return ParentTRelationship(params);
	case RelationshipType::USES:
		return UsesRelationship(params);
	case RelationshipType::MODIFIES:
		return ModifiesRelationship(params);
	}
	throw - 1;
}

Relationship::Relationship(const Relationship& r)
{
	type = r.type;
	params = r.params;
}

vector<Parameter*> Relationship::getAllUncheckedSynonyms()
{
	vector<Parameter*> synonyms;
	for (int i = 0; i < params.size(); i++) {
		if (params.at(i).isUncheckedSynonym()) {
			synonyms.push_back(&params.at(i));
		}
	}
	return synonyms;
}

vector<Parameter>& Relationship::getParameters() {
    return params;
}

bool Relationship::operator==(const Relationship& r) const
{
	return type == r.type && params == r.params;
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
	{"Follows" , RelationshipType::FOLLOWS},
	{"Follows*", RelationshipType::FOLLOWST},
	{"Parent", RelationshipType::PARENT},
	{"Parent*", RelationshipType::PARENTT},
	{"Uses", RelationshipType::USES},
	{"Modifies", RelationshipType::MODIFIES}
};
