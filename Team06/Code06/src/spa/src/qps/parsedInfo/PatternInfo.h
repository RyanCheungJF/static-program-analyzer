#ifndef SPA_QPS_PATTERNINFO_H
#define SPA_QPS_PATTERNINFO_H
#include<string>
using namespace std;

class PatternInfo {
public:
	string synAssign;
	string entRef;
	string pattern;

	PatternInfo();
	PatternInfo(const PatternInfo&);
	PatternInfo(string, string, string);
};

#endif // !SPA_QPS_PATTERNINFO_H
