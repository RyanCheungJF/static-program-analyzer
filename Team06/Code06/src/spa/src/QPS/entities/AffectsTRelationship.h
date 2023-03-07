#ifndef SPA_QPS_AFFECTSTRELATIONSHIP_H
#define SPA_QPS_AFFECTSTRELATIONSHIP_H
#include "Relationship.h"
#include "exceptions/SyntaxException.h"

using namespace std;

class AffectsTRelationship : public Relationship {
public:
    AffectsTRelationship(vector<Parameter>&);
    bool validateParams();
};

#endif // SPA_QPS_AFFECTSTRELATIONSHIP_H