#ifndef SPA_QPS_FOLLOWSTRELATIONSHIP_H
#define SPA_QPS_FOLLOWSTRELATIONSHIP_H
#include "Relationship.h"
#include "exceptions/SyntaxException.h"

using namespace std;

class FollowsTRelationship : public Relationship {
public:
  FollowsTRelationship(vector<Parameter>);
  bool validateParams();

private:
  bool validateSyntax(vector<Parameter>);
};

#endif // SPA_QPS_FOLLOWSTRELATIONSHIP_H