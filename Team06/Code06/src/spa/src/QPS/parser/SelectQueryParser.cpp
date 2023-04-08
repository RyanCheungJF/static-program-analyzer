#include "SelectQueryParser.h"

// assumes select clause is valid
// does not check if synonym is declared
Query SelectQueryParser::parse(string selectQuery) {
    vector<string> wordList = stringToWordList(selectQuery);
    map<ClauseType, vector<int>> clauseStarts = getClauseStarts(wordList);
    vector<tuple<ClauseType, int, int>> clausePositions = getClausePositions(clauseStarts, wordList.size());
    vector<Parameter> selectParams;
    vector<shared_ptr<Relationship>> relations;
    vector<shared_ptr<Relationship>> tempRelations;
    vector<Pattern> patterns;
    vector<Pattern> tempPatterns;
    vector<Comparison> comparisons;
    vector<Comparison> tempComparisons;
    bool isSelectTuple;
    for (tuple<ClauseType, int, int> clause : clausePositions) {
        ClauseType ct;
        int clauseStart, clauseEnd;
        tie(ct, clauseStart, clauseEnd) = clause;
        switch (ct) {
        case ClauseType::SUCH_THAT:
            tempRelations = parseSuchThatClause(wordList, clauseStart, clauseEnd);
            relations.insert(relations.end(), tempRelations.begin(), tempRelations.end());
            break;
        case ClauseType::PATTERN:
            tempPatterns = parsePatternClause(wordList, clauseStart, clauseEnd);
            patterns.insert(patterns.end(), tempPatterns.begin(), tempPatterns.end());
            break;
        case ClauseType::WITH:
            tempComparisons = parseWithClause(wordList, clauseStart, clauseEnd);
            comparisons.insert(comparisons.end(), tempComparisons.begin(), tempComparisons.end());
            break;
        case ClauseType::SELECT:
            tie(selectParams, isSelectTuple) = parseSelectClause(wordList, clauseStart, clauseEnd);
            break;
        default:
            break;
        }
    }
    Query query(selectParams, relations, patterns, comparisons, isSelectTuple);
    return query;
}

/*
splits the select, such that, and pattern clauses,
assuming we only have at most one of each type of clauses

@returns: a vector of size 3, containing list of select clause, list of such
that clause, and list of pattern clause in that order
*/
map<ClauseType, vector<int>> SelectQueryParser::getClauseStarts(vector<string>& wordList) {
    vector<int> suchThatStart = findSuchThat(wordList);
    vector<int> patternStart = findPattern(wordList);
    vector<int> withStart = findWith(wordList);
    vector<int> selectStart{0};

    map<ClauseType, vector<int>> res{{ClauseType::SELECT, selectStart},
                                     {ClauseType::SUCH_THAT, suchThatStart},
                                     {ClauseType::PATTERN, patternStart},
                                     {ClauseType::WITH, withStart}};
    return res;
}

vector<tuple<ClauseType, int, int>> SelectQueryParser::getClausePositions(map<ClauseType, vector<int>> clauseStarts,
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
        int clauseEnd = (i != clauseStartsVector.size() - 1) ? clauseStartsVector[i + (size_t)1] : wordListLength;
        ClauseType ct = startToClauseType[clauseStart];
        res.push_back({ct, clauseStart, clauseEnd});
    }

    return res;
}

/*
assumes start and end won't be -1 i.e. select clause must exist
*/
tuple<vector<Parameter>, bool> SelectQueryParser::parseSelectClause(vector<string>& wordList, int start, int end) {
    vector<Parameter> params;
    if (end - start < 2) {
        // select clause does not exist
        throw SyntaxException();
    }
    if (!isSelect(wordList[start])) {
        throw InternalException("Error: SelectQueryParser.parseSelectClause bad "
                                "start position for wordList");
    }
    start = start + 1;
    bool isSelectTuple = isTupleStart(wordList[start]);
    if (isSelectTuple) {
        // it is a tuple select clause
        vector<string> paramStrings;
        string tupleString;
        for (start; start < end; start++) {
            tupleString += " " + wordList[start];
        }
        tie(ignore, paramStrings) =
            extractParameters(tupleString, AppConstants::STRING_LESS, AppConstants::STRING_GREATER, ",");
        for (string elemString : paramStrings) {
            if (!isElem(elemString)) {
                throw SyntaxException();
            }
            Parameter param = parseParameter(elemString);
            params.push_back(param);
        }
        return make_tuple(params, isSelectTuple);
    }
    // single select parameter
    string elemString = "";
    for (int i = start; i < end; i++) {
        elemString += wordList[i] + " ";
    }
    if (!isElem(elemString)) {
        throw SyntaxException();
    }
    Parameter param = parseParameter(elemString);
    params.push_back(param);
    return make_tuple(params, isSelectTuple);
}

vector<shared_ptr<Relationship>> SelectQueryParser::parseSuchThatClause(vector<string>& wordList, int start, int end) {
    vector<shared_ptr<Relationship>> res;
    if (end <= start) {
        throw InternalException("Error: SelectQueryParser.parseSuchThatClause bad "
                                "start position and end position");
    }
    if (end - start < 3) {
        throw SyntaxException();
    }

    int curIndex = start + 2;
    vector<string> unparsedRelRef = splitClauseByAnds(wordList, curIndex, end, hasCorrectRelRefOrPatternForm);

    vector<tuple<string, vector<string>>> relRefParams;
    for (int i = 0; i < unparsedRelRef.size(); i++) {
        relRefParams.push_back(extractParameters(unparsedRelRef[i], AppConstants::STRING_LEFT_PARENTHESIS,
                                                 AppConstants::STRING_RIGHT_PARENTHESIS, ","));
    }

    for (int i = 0; i < relRefParams.size(); i++) {
        // construct relations based on extracted params, can consider extracting
        string rel;
        vector<string> paramStrings;
        vector<Parameter> params;
        tie(rel, paramStrings) = relRefParams.at(i);
        for (string pString : paramStrings) {
            Parameter p = Parameter::makeParameter(pString);
            params.push_back(p);
        }
        res.push_back(Relationship::makeRelationship(rel, params));
        // construction ends here
    }

    return res;
}

vector<Pattern> SelectQueryParser::parsePatternClause(vector<string>& wordList, int start, int end) {
    vector<Pattern> res;
    if (end <= start) {
        throw InternalException("Error: SelectQueryParser.parsePatternClause bad "
                                "start position and end position");
    }
    if (end - start < 2) {
        throw SyntaxException();
    }

    int curIndex = start + 1;
    vector<string> unparsedPatterns = splitClauseByAnds(wordList, curIndex, end, hasCorrectRelRefOrPatternForm);

    vector<tuple<string, vector<string>>> patternParams;
    for (const auto& unparsedPattern : unparsedPatterns) {
        patternParams.push_back(extractParameters(unparsedPattern, AppConstants::STRING_LEFT_PARENTHESIS,
                                                  AppConstants::STRING_RIGHT_PARENTHESIS, ","));
    }

    for (tuple<string, vector<string>> t : patternParams) {
        // construct patterns based on extracted params
        string patternDsgEntString, entRefString;
        vector<string> paramStrings;
        tie(patternDsgEntString, paramStrings) = t;
        if (paramStrings.size() < 1) {
            throw SyntaxException();
        }
        entRefString = paramStrings[0];
        Parameter patternSyn = Parameter::makeParameter(patternDsgEntString);
        Parameter entRef = Parameter::makeParameter(entRefString);
        vector<string> exprSpecs;
        for (int i = 1; i < paramStrings.size(); i++) {
            exprSpecs.push_back(paramStrings[i]);
        }
        // construction ends here
        res.push_back(Pattern::makePattern(patternSyn, entRef, exprSpecs));
    }

    return res;
}

vector<Comparison> SelectQueryParser::parseWithClause(vector<string>& wordList, int start, int end) {
    vector<Comparison> res;

    if (end <= start) {
        throw InternalException("Error: SelectQueryParser.parseWithClause bad "
                                "start position and end position");
    }
    if (end - start < 2) {
        throw SyntaxException();
    }
    int curIndex = start + 1;
    vector<string> unparsedComparisons = splitClauseByAnds(wordList, curIndex, end, hasCorrectAttrCompForm);
    for (string compString : unparsedComparisons) {
        vector<string> splitParams = stringToWordListByDelimiter(compString, AppConstants::OP_EQUALS);
        if (!isRef(splitParams[0]) || !isRef(splitParams[1])) {
            throw SyntaxException();
        }
        Parameter leftP = parseParameter(splitParams[0]);
        Parameter rightP = parseParameter(splitParams[1]);
        Comparison comp = Comparison::makeComparison(AppConstants::OP_EQUALS, leftP, rightP);
        res.push_back(comp);
    }
    return res;
}

vector<ClauseType> SelectQueryParser::getAllClauseTypes() {
    return vector<ClauseType>{ClauseType::SELECT, ClauseType::SUCH_THAT, ClauseType::PATTERN, ClauseType::WITH};
}

Parameter SelectQueryParser::parseParameter(string paramString) {
    string paramName;
    int index;
    bool found;
    tie(paramName, index, found) = extractSubStringUntilDelimiter(trim(paramString), 0, ".");
    if (!found) {
        return Parameter::makeParameter(paramName);
    }
    string attributeType = paramString.substr(index, paramString.length() - index);
    return Parameter::makeParameter(trim(paramName), trim(attributeType));
}
vector<string> SelectQueryParser::splitClauseByAnds(vector<string>& wordList, int start, int end,
                                                    function<bool(string)> formChecker) {
    vector<int> ands = findAnds(wordList, start, end);
    vector<string> res;
    string condString = "";
    int curIndex = start;
    for (int i = 0; i < ands.size(); i++) {
        for (int j = curIndex; j < ands[i]; j++) {
            // spaces are removed when converting into wordlist, need to add it back
            condString += wordList[j];
        }
        if (!formChecker(condString)) {
            condString += " ";
            curIndex = ands[i];
            continue;
        }
        res.push_back(condString);
        condString = "";
        curIndex = ands[i] + 1;
    }

    while (curIndex < end) {
        condString += wordList[curIndex] + " ";
        curIndex++;
    }
    condString = trim(condString);
    if (!formChecker(condString)) {
        throw SyntaxException();
    }
    res.push_back(condString);
    return res;
}
