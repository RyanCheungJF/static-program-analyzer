#ifndef SPA_QPS_CALLSTRELATIONSHIP_H
#define SPA_QPS_CALLSTRELATIONSHIP_H
#include "Relationship.h"
#include "exceptions/SyntaxException.h"

using namespace std;

class CallsTRelationship : public Relationship {
public:
  CallsTRelationship(vector<Parameter> &);
  bool validateParams();

private:
  bool validateSyntax(vector<Parameter> &);
};

#endif // SPA_QPS_CALLSTRELATIONSHIP_H