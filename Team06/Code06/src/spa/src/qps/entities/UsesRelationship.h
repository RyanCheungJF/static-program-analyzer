#ifndef SPA_QPS_USESRELATIONSHIP_H
#define SPA_QPS_USESRELATIONSHIP_H
#include "Relationship.h"

using namespace std;

class UsesRelationship : public Relationship {
public:
	UsesRelationship(vector<Parameter>&);
private:
	bool validateParams(vector<Parameter>&);
};

#endif //SPA_QPS_USESRELATIONSHIP_H