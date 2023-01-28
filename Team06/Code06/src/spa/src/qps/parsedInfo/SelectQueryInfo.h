#ifndef SPA_QPS_SELECTQUERYINFO_H
#define SPA_QPS_SELECTQUERYINFO_H
#include <PatternInfo.h>
#include <SelectClauseInfo.h>
#include <SuchThatInfo.h>
#include <vector>
using namespace std;

class SelectQueryInfo {
public:
	SelectClauseInfo selectClauseInfo;
	SuchThatInfo suchThatInfo;
	PatternInfo patternInfo;

	SelectQueryInfo(SelectClauseInfo, SuchThatInfo, PatternInfo);
};

#endif // !SPA_QPS_SELECTQUERYINFO_H
