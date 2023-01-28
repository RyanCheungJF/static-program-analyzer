#ifndef SPA_SELECTCLAUSEPARSER_H
#define SPA_SELECTCLAUSEPARSER_H
#include "qps/parsedInfo/SelectClauseInfo.h"
#include "qps/parsedInfo/SelectQueryInfo.h"
#include "parserUtil.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

class SelectClauseParser {
public:
	SelectQueryInfo parse(string selectQuery);
private:
	vector<int> getClauseStarts(vector<string> &wordList);
	vector<int> getClauseEnds(vector<int> clauseStarts, int wordListLength);
	SelectClauseInfo parseSelectClause(vector<string> &wordList, int start, int end);
	SuchThatInfo parseSuchThatClause(vector<string> &wordList, int start, int end);
	PatternInfo parsePatternClause(vector<string> &wordList, int start, int end);
};

#endif //SPA_SELECTCLAUSEPARSER_H