#ifndef SPA_SELECTCLAUSEPARSER_H
#define SPA_SELECTCLAUSEPARSER_H
#include "ParserUtil.h"
#include "exceptions/Exception.h"
#include "QPS/entities/Query.h"
#include "QPS/entities/Relationship.h"
#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

enum ClauseType { SELECT, SUCH_THAT, PATTERN, WITH };

class SelectQueryParser {
public:
  Query parse(string selectQuery);

private:
  map<ClauseType, vector<int>> getClauseStarts(vector<string> &wordList);
  vector<tuple<ClauseType, int, int>>
  getClausePositions(map<ClauseType, vector<int>> clauseStarts,
                     int wordListLength);
  vector<Parameter> parseSelectClause(vector<string> &wordList, int start,
                                      int end);
  vector<shared_ptr<Relationship>> parseSuchThatClause(vector<string> &wordList,
                                                       int start, int end);
  vector<Pattern> parsePatternClause(vector<string> &wordList, int start,
                                     int end);
  vector<Parameter> extractSelectTuple(vector<string> &wordList, int start, int end);
  vector<ClauseType> getAllClauseTypes();
};

#endif // SPA_SELECTCLAUSEPARSER_H