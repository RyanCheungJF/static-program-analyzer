#ifndef SPA_QPS_FOLLOWSRELATIONSHIP_H
#define SPA_QPS_FOLLOWSRELATIONSHIP_H
#include "Relationship.h"
#include "exceptions/SyntaxException.h"

using namespace std;

class FollowsRelationship : public Relationship {
public:
  FollowsRelationship(vector<Parameter>);
  bool validateParams();

private:
  bool validateSyntax(vector<Parameter>);
};
#endif // !SPA_QPS_FOLLOWSRELATIONSHIP_H
