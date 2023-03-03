#ifndef SPA_QPS_NEXTTRELATIONSHIP_H
#define SPA_QPS_NEXTTRELATIONSHIP_H
#include "Relationship.h"
#include "exceptions/SyntaxException.h"

using namespace std;

class NextTRelationship : public Relationship {
public:
  NextTRelationship(vector<Parameter>);
  bool validateParams();

private:
  bool validateSyntax(vector<Parameter>);
};
#endif // !SPA_QPS_NEXTTRELATIONSHIP_H