#ifndef SPA_QPS_MODIFIESRELATIONSHIP_H
#define SPA_QPS_MODIFIESRELATIONSHIP_H
#include "Relationship.h"

using namespace std;

class ModifiesRelationship : public Relationship {
public:
	ModifiesRelationship(vector<Parameter>&);
private:
	bool validateParams(vector<Parameter>&);
};

#endif //SPA_QPS_MODIFIESRELATIONSHIP_H