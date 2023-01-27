#ifndef SPA_SELECTCLAUSEPARSER_H
#define SPA_SELECTCLAUSEPARSER_H
#include "qps/parsedInfo/SelectClauseInfo.h"
#include "qps/parsedInfo/SelectQueryInfo.h"
#include <string>
#include <vector>
using namespace std;

class SelectClauseParser {
public:
	SelectQueryInfo parse(string selectQuery);
private:
	vector<vector<string>> getClauses(string selectQuery);
	SelectClauseInfo parseSelectClause(string selectClause);
	SuchThatInfo parseSuchThatClause(string suchThatClause);
	PatternInfo parsePatternClause(string patternClause);
};

#endif //SPA_SELECTCLAUSEPARSER_H