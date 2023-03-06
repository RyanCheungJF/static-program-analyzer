#ifndef SPA_QPS_AFFECTSRELATIONSHIP_H
#define SPA_QPS_AFFECTSRELATIONSHIP_H
#include "Relationship.h"
#include "exceptions/SyntaxException.h"

using namespace std;

class AffectsRelationship : public Relationship {
public:
    AffectsRelationship(vector<Parameter>&);
    bool validateParams();
};

#endif // SPA_QPS_AFFECTSRELATIONSHIP_H