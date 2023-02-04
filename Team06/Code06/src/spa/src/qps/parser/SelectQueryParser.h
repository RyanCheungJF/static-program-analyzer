#ifndef SPA_SELECTCLAUSEPARSER_H
#define SPA_SELECTCLAUSEPARSER_H
#include "exceptions/Exception.h"
#include "qps/entities/Relationship.h"
#include "qps/entities/Query.h"
#include "parserUtil.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <tuple>

using namespace std;

class SelectQueryParser {
public:
	Query parse(string selectQuery);
private:
	vector<int> getClauseStarts(vector<string> &wordList);
	vector<int> getClauseEnds(vector<int> clauseStarts, int wordListLength);
	vector<Parameter> parseSelectClause(vector<string> &wordList, int start, int end);
	vector<Relationship> parseSuchThatClause(vector<string> &wordList, int start, int end);
	vector<Pattern> parsePatternClause(vector<string> &wordList, int start, int end);
};

#endif //SPA_SELECTCLAUSEPARSER_H