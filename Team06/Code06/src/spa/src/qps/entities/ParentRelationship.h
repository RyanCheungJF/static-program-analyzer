#ifndef SPA_QPS_PARENTRELATIONSHIP_H
#define SPA_QPS_PARENTRELATIONSHIP_H
#include "Relationship.h"

using namespace std;

class ParentRelationship : public Relationship {
public:
	ParentRelationship(vector<Parameter>&);
private:
	bool validateParams(vector<Parameter>&);
};

#endif //SPA_QPS_PARENTRELATIONSHIP_H