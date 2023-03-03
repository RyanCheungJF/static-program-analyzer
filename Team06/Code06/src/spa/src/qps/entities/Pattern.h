#ifndef SPQ_QPS_PATTERN_H
#define SPQ_QPS_PATTERN_H
#include "Parameter.h"
#include "../../exceptions/SyntaxException.h"
using namespace std;

class Pattern {
public:
	Parameter patternSyn;
	Parameter entRef;
    Parameter ifsParam = Parameter("", "");
	string pattern;
	Pattern();
	Pattern(const Pattern&);
	Pattern(Parameter, Parameter, string);
    Pattern(Parameter, Parameter, string, Parameter);
	Parameter* getPatternSyn();
	Parameter* getEntRef();
	bool validateParams();
};

#endif //SPQ_QPS_PATTERN_H