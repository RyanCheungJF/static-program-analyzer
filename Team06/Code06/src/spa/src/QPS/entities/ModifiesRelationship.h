#ifndef SPA_QPS_MODIFIESRELATIONSHIP_H
#define SPA_QPS_MODIFIESRELATIONSHIP_H
#include "Relationship.h"
#include "exceptions/SyntaxException.h"

using namespace std;

class ModifiesRelationship : public Relationship {
public:
  ModifiesRelationship(vector<Parameter> &);
  bool validateParams();
};

#endif // SPA_QPS_MODIFIESRELATIONSHIP_H