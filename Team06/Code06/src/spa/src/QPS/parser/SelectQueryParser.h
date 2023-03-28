#ifndef SPA_SELECTCLAUSEPARSER_H
#define SPA_SELECTCLAUSEPARSER_H
#include <algorithm>
#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "ParserUtil.h"
#include "QPS/entities/Comparison.h"
#include "QPS/entities/Query.h"
#include "QPS/entities/Relationship.h"
#include "exceptions/Exception.h"

using namespace std;

enum class ClauseType {
    SELECT,
    SUCH_THAT,
    PATTERN,
    WITH
};

class SelectQueryParser {
public:
    Query parse(string selectQuery);
    Parameter parseParameter(string paramString);

private:
    map<ClauseType, vector<int>> getClauseStarts(vector<string>& wordList);
    vector<tuple<ClauseType, int, int>> getClausePositions(map<ClauseType, vector<int>> clauseStarts,
                                                           int wordListLength);
    vector<Parameter> parseSelectClause(vector<string>& wordList, int start, int end);
    vector<shared_ptr<Relationship>> parseSuchThatClause(vector<string>& wordList, int start, int end);
    vector<Pattern> parsePatternClause(vector<string>& wordList, int start, int end);
    vector<Comparison> parseWithClause(vector<string>& wordList, int start, int end);
    vector<Parameter> extractSelectTuple(vector<string>& wordList, int start, int end);
    vector<ClauseType> getAllClauseTypes();
    vector<string> splitClauseByAnds(vector<string>& wordList, int start, int end, function<bool(string)> formChecker);
};

#endif // SPA_SELECTCLAUSEPARSER_H