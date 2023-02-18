#ifndef SPA_QPS_PARENTTRELATIONSHIP_H
#define SPA_QPS_PARENTTRELATIONSHIP_H
#include "Relationship.h"
#include "exceptions/SyntaxException.h"

using namespace std;

class ParentTRelationship : public Relationship {
public:
	ParentTRelationship(vector<Parameter>&);
	bool validateParams();
private:
	bool validateSyntax(vector<Parameter>&);
};

#endif //SPA_QPS_PARENTTRELATIONSHIP_H