#include "SelectQueryParser.h"

// assumes select clause is valid
// does not check if synonym is declared
Query SelectQueryParser::parse(string selectQuery) {
  vector<string> wordList = stringToWordList(selectQuery);
  map<ClauseType, vector<int>> clauseStarts = getClauseStarts(wordList);
  vector<tuple<ClauseType, int, int>> clausePositions =
      getClausePositions(clauseStarts, wordList.size());
  vector<Parameter> selectParams;
  vector<shared_ptr<Relationship>> relations;
  vector<shared_ptr<Relationship>> tempRelations;
  vector<Pattern> patterns;
  vector<Pattern> tempPatterns;
  for (tuple<ClauseType, int, int> clause : clausePositions) {
    ClauseType ct;
    int clauseStart, clauseEnd;
    tie(ct, clauseStart, clauseEnd) = clause;
    switch (ct) {
    case SUCH_THAT:
      tempRelations = parseSuchThatClause(wordList, clauseStart, clauseEnd);
      relations.insert(relations.end(), tempRelations.begin(),
                       tempRelations.end());
      break;
    case PATTERN:
      tempPatterns = parsePatternClause(wordList, clauseStart, clauseEnd);
      patterns.insert(patterns.end(), tempPatterns.begin(), tempPatterns.end());
      break;
    case WITH:
      break;
    case SELECT:
      selectParams = parseSelectClause(wordList, clauseStart, clauseEnd);
      break;
    default:
      break;
    }
  }
  Query query(selectParams, relations, patterns);
  return query;
}

/*
splits the select, such that, and pattern clauses,
assuming we only have at most one of each type of clauses

@returns: a vector of size 3, containing list of select clause, list of such
that clause, and list of pattern clause in that order
*/
map<ClauseType, vector<int>>
SelectQueryParser::getClauseStarts(vector<string> &wordList) {
  vector<int> suchThatStart = findSuchThat(wordList);
  vector<int> patternStart = findPattern(wordList);
  vector<int> withStart{};
  vector<int> selectStart{0};

  map<ClauseType, vector<int>> res{{SELECT, selectStart},
                                   {SUCH_THAT, suchThatStart},
                                   {PATTERN, patternStart},
                                   {WITH, withStart}};
  return res;
}

vector<tuple<ClauseType, int, int>>
SelectQueryParser::getClausePositions(map<ClauseType, vector<int>> clauseStarts,
                                      int wordListLength) {
  vector<ClauseType> allClauseTypes = getAllClauseTypes();
  map<int, ClauseType> startToClauseType;
  vector<int> clauseStartsVector;
  for (ClauseType ct : allClauseTypes) {
    vector<int> starts = clauseStarts[ct];
    for (int s : starts) {
      clauseStartsVector.push_back(s);
      startToClauseType.insert({s, ct});
    }
  }

  sort(clauseStartsVector.begin(), clauseStartsVector.end());
  vector<tuple<ClauseType, int, int>> res;
  for (int i = 0; i < clauseStartsVector.size(); i++) {
    int clauseStart = clauseStartsVector[i];
    int clauseEnd = (i != clauseStartsVector.size() - 1)
                        ? clauseStartsVector[i + (size_t)1]
                        : wordListLength;
    ClauseType ct = startToClauseType[clauseStart];
    res.push_back({ct, clauseStart, clauseEnd});
  }

  return res;
}

/*
assumes start and end won't be -1 i.e. select clause must exist
*/
vector<Parameter> SelectQueryParser::parseSelectClause(vector<string> &wordList,
                                                       int start, int end) {
  vector<Parameter> params;
  if (end - start != 2) {
    // select clause does not exist
    throw SyntaxException();
  }
  if (!isSelect(wordList[start])) {
    throw InternalException("Error: SelectQueryParser.parseSelectClause bad "
                            "start position for wordList");
  }
  if (!isSynonym(wordList[end - (size_t)1])) {
    // bad select parameter
    throw SyntaxException();
  }
  Parameter param =
      Parameter::makeParameter(wordList[1], AppConstants::SYNONYM);
  params.push_back(param);
  return params;
}

vector<shared_ptr<Relationship>>
SelectQueryParser::parseSuchThatClause(vector<string> &wordList, int start,
                                       int end) {
  vector<shared_ptr<Relationship>> res;
  if (start == -1 && end == -1) {
    return res;
  }
  if (end <= start) {
    throw InternalException("Error: SelectQueryParser.parseSuchThatClause bad "
                            "start position and end position");
  }
  if (end - start < 3) {
    throw SyntaxException();
  }

  int curIndex = start + 2;
  // Duplicate logic as pattern, may want to extract out
  // vector<string> SplitClauseByAnds(vector<string> wordList, function<bool(string)> formChecker)
  vector<int> ands = findAnds(wordList, start, end);
  vector<string> unparsedRelRef;
  string condString = "";
  for (int i = 0; i < ands.size(); i++) {
    for (int j = curIndex; j < ands[i]; j++) {
      condString += wordList[j];
    }
    if (!hasCorrectRelRefOrPatternForm(condString)) {
      curIndex = ands[i];
      continue;
    }
    unparsedRelRef.push_back(condString);
    condString = "";
    curIndex = ands[i] + 1;
  }

  while (curIndex < end) {
    condString += wordList[curIndex];
    curIndex++;
  }
  if (!hasCorrectRelRefOrPatternForm(condString)) {
    throw SyntaxException();
  }
  unparsedRelRef.push_back(condString);
  // Duplicate til here

  vector<tuple<string, vector<string>>> relRefParams;
  for (int i = 0; i < unparsedRelRef.size(); i++) {
    relRefParams.push_back(extractParameters(unparsedRelRef[i], "(", ")", ","));
  }

  for (int i = 0; i < relRefParams.size(); i++) {
    string rel;
    vector<string> paramStrings;
    vector<Parameter> params;
    tie(rel, paramStrings) = relRefParams.at(i);
    for (string pString : paramStrings) {
      Parameter p = Parameter::makeParameter(pString);
      params.push_back(p);
    }
    res.push_back(Relationship::makeRelationship(rel, params));
  }

  return res;
}

vector<Pattern> SelectQueryParser::parsePatternClause(vector<string> &wordList,
                                                      int start, int end) {
  vector<Pattern> res;
  if (start == -1 && end == -1) {
    return res;
  }
  if (end <= start) {
    throw InternalException("Error: SelectQueryParser.parsePatternClause bad "
                            "start position and end position");
  }
  if (end - start < 2) {
    throw SyntaxException();
  }

  int curIndex = start + 1;
  //Duplicate logic as such that, may want to extract out
  vector<int> ands = findAnds(wordList, start, end);
  vector<string> unparsedPatterns;
  string condString;
  for (int i = 0; i < ands.size(); i++) {
    for (int j = curIndex; j < ands[i]; j++) {
      condString += wordList[j];
    }
    if (!hasCorrectRelRefOrPatternForm(condString)) {
      curIndex = ands[i];
      continue;
    }
    unparsedPatterns.push_back(condString);
    condString = "";
    curIndex = ands[i] + 1;
  }

  while (curIndex < end) {
    condString += wordList[curIndex];
    curIndex++;
  }
  if (!hasCorrectRelRefOrPatternForm(condString)) {
    throw SyntaxException();
  }
  unparsedPatterns.push_back(condString);
  //Duplicate til here

  vector<tuple<string, vector<string>>> patternParams;
  for (const auto &unparsedPattern : unparsedPatterns) {
    patternParams.push_back(extractParameters(unparsedPattern, "(", ")", ","));
  }

  for (tuple<string, vector<string>> t : patternParams) {
    string patternDsgEntString, entRefString, patternString, ifsString;
    vector<string> paramStrings;
    tie(patternDsgEntString, paramStrings) = t;
    //Do these validations using a pattern validator
    try {
      entRefString = paramStrings.at(0);
      patternString = paramStrings.at(1);
      ifsString = paramStrings.size() == 3 ? paramStrings.at(2) : "";
    } catch (out_of_range e) {
      throw SyntaxException();
    }
    if (!isSynonym(patternDsgEntString)) {
      throw SyntaxException();
    }

    if (!isEntRef(entRefString)) {
      throw SyntaxException();
    }

    if (!isExprSpec(patternString)) {
      throw SyntaxException();
    }
    Parameter patternDsgEnt(patternDsgEntString, ParameterType::SYNONYM);
    Parameter entRef(removeCharFromString(entRefString, '\"'),
                     Parameter::guessParameterType(entRefString));
    Parameter ifParam("", ParameterType::UNKNOWN);
    if (!ifsString.empty()) {
      ifParam = *new Parameter("", ParameterType::WILDCARD);
    }
    res.emplace_back(patternDsgEnt, entRef,
                     removeCharFromString(patternString, '\"'), ifParam);
  }
  return res;
}

vector<ClauseType> SelectQueryParser::getAllClauseTypes() {
  return vector<ClauseType>{SELECT, SUCH_THAT, PATTERN, WITH};
}
