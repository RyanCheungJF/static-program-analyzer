#ifndef SPA_QPS_PARENTTRELATIONSHIP_H
#define SPA_QPS_PARENTTRELATIONSHIP_H
#include "Relationship.h"

using namespace std;

class ParentTRelationship : public Relationship {
public:
	ParentTRelationship(vector<Parameter>&);
private:
	bool validateParams(vector<Parameter>&);
};

#endif //SPA_QPS_PARENTTRELATIONSHIP_H