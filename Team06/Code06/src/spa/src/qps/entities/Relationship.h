#ifndef SPA_QPS_RELATIONSHIP_H
#define SPA_QPS_RELATIONSHIP_H
#include <string>
#include "Parameter.h"
using namespace std;

class Relationship {
public:
	string type;
	int paramCount;
	vector<Parameter> params;
};

#endif // !SPA_QPS_RELATIONSHIP_H
