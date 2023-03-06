#ifndef SPA_QPS_USESRELATIONSHIP_H
#define SPA_QPS_USESRELATIONSHIP_H
#include "Relationship.h"
#include "exceptions/SyntaxException.h"

using namespace std;

class UsesRelationship : public Relationship {
public:
    UsesRelationship(vector<Parameter>&);
    bool validateParams();
};

#endif // SPA_QPS_USESRELATIONSHIP_H