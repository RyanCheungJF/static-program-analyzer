#ifndef SPA_QPS_SELECTQUERYINFO_H
#define SPA_QPS_SELECTQUERYINFO_H
#include <PatternInfo.h>
#include <SelectClauseInfo.h>
#include <SuchThatInfo.h>
#include <vector>
using namespace std;

class SelectQueryInfo {
public:
	vector<SelectClauseInfo> selectClauseInfos;
	vector<SuchThatInfo> suchThatInfos;
	vector<PatternInfo> patternInfos;
};

#endif // !SPA_QPS_SELECTQUERYINFO_H
