#ifndef SPA_QPS_FOLLOWSRELATIONSHIP_H
#define SPA_QPS_FOLLOWSRELATIONSHIP_H
#include "Relationship.h"

using namespace std;

class FollowsRelationship : public Relationship {
public:
	FollowsRelationship(vector<Parameter>);
private:
	bool validateParams(vector<Parameter>);
};
#endif // !SPA_QPS_FOLLOWSRELATIONSHIP_H