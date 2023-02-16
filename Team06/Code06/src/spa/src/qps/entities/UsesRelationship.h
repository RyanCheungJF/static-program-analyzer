#ifndef SPA_QPS_USESRELATIONSHIP_H
#define SPA_QPS_USESRELATIONSHIP_H
#include "Relationship.h"

using namespace std;

class UsesRelationship : public Relationship {
public:
	UsesRelationship(vector<Parameter>&);
	bool validateParams();
private:
	bool validateSyntax(vector<Parameter>&);
};

#endif //SPA_QPS_USESRELATIONSHIP_H