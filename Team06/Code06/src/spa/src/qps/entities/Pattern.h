#ifndef SPQ_QPS_PATTERN_H
#define SPQ_QPS_PATTERN_H
#include "Parameter.h"
using namespace std;

class Pattern {
public:
	Parameter synAssign;
	Parameter entRef;
	string pattern;
	Pattern();
	Pattern(const Pattern&);
	Pattern(Parameter, Parameter, string);
	Parameter* getSynAssign();
	Parameter* getEntRef();
	bool validateParams();
};

#endif //SPQ_QPS_PATTERN_H