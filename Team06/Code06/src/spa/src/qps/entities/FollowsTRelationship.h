#ifndef SPA_QPS_FOLLOWSTRELATIONSHIP_H
#define SPA_QPS_FOLLOWSTRELATIONSHIP_H
#include "Relationship.h"

using namespace std;

class FollowsTRelationship : public Relationship {
public:
	FollowsTRelationship(vector<Parameter>);
private:
	bool validateParams(vector<Parameter>);
};

#endif //SPA_QPS_FOLLOWSTRELATIONSHIP_H