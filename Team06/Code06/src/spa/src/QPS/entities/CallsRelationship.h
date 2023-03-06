#ifndef SPA_QPS_CALLSRELATIONSHIP_H
#define SPA_QPS_CALLSRELATIONSHIP_H
#include "Relationship.h"
#include "exceptions/SyntaxException.h"

using namespace std;

class CallsRelationship : public Relationship {
public:
  CallsRelationship(vector<Parameter> &);
  bool validateParams();

private:
};

#endif // SPA_QPS_CALLSRELATIONSHIP_H