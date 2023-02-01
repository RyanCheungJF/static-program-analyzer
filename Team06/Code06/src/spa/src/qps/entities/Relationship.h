#ifndef SPA_QPS_RELATIONSHIP_H
#define SPA_QPS_RELATIONSHIP_H
#include <string>
#include "Parameter.h"
#include "FollowsRelationship.h"
using namespace std;

enum class RelationshipType {
	FOLLOWS,
	RELATIONSHIP,
	UNKNOWN
};

class Relationship {
public:
	static Relationship makeRelationship(string type, vector<Parameter> params);
	RelationshipType type;
	vector<Parameter> params;
	virtual bool validateParams(vector<Parameter> params);
private:
	static const unordered_map<string, RelationshipType> stringToTypeMap;
	static RelationshipType stringToType(string);
};

#endif // !SPA_QPS_RELATIONSHIP_H
