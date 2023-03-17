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
    for (tuple<ClauseType, int, int> clause : clausePositions) {
        ClauseType ct;
        int clauseStart, clauseEnd;
        tie(ct, clauseStart, clauseEnd) = clause;
        switch (ct) {
        case SUCH_THAT:
            tempRelations = parseSuchThatClause(wordList, clauseStart, clauseEnd);
            relations.insert(relations.end(), tempRelations.begin(), tempRelations.end());
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
map<ClauseType, vector<int>> SelectQueryParser::getClauseStarts(vector<string>& wordList) {
    vector<int> suchThatStart = findSuchThat(wordList);
    vector<int> patternStart = findPattern(wordList);
    vector<int> withStart{};
    vector<int> selectStart{0};

    map<ClauseType, vector<int>> res{
        {SELECT, selectStart}, {SUCH_THAT, suchThatStart}, {PATTERN, patternStart}, {WITH, withStart}};
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

vector<Parameter> SelectQueryParser::extractSelectTuple(vector<string>& wordList, int start, int end) {
    string tupleString;
    vector<Parameter> params;
    for (start; start < end; start++) {
        // recreates tuple string with whitespace removed
        tupleString += wordList[start];
    }
    if (tupleString.back() != '>') {
        throw SyntaxException();
    }
    // gets rid of <>
    tupleString.pop_back();
    tupleString.erase(0, 1);
    vector<string> synonyms = stringToWordListByDelimiter(tupleString, ",");
    for (string synonym : synonyms) {
        if (!isSynonym(synonym)) {
            throw SyntaxException();
        }
        Parameter param = Parameter::makeParameter(synonym, AppConstants::SYNONYM);
        params.push_back(param);
    }
    return params;
}

/*
assumes start and end won't be -1 i.e. select clause must exist
*/
vector<Parameter> SelectQueryParser::parseSelectClause(vector<string>& wordList, int start, int end) {
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
    if (isTupleStart(wordList[start])) {
        // it is a tuple select clause
        params = extractSelectTuple(wordList, start, end);
        return params;
    }
    else if (isSynonym(wordList[start])) {
        // single select parameter
        Parameter param = Parameter::makeParameter(wordList[1], AppConstants::SYNONYM);
        params.push_back(param);
        return params;
    }
    throw SyntaxException();
}

vector<shared_ptr<Relationship>> SelectQueryParser::parseSuchThatClause(vector<string>& wordList, int start, int end) {
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
    vector<string> unparsedRelRef = splitClauseByAnds(wordList, curIndex, end, hasCorrectRelRefOrPatternForm);

    vector<tuple<string, vector<string>>> relRefParams;
    for (int i = 0; i < unparsedRelRef.size(); i++) {
        relRefParams.push_back(extractParameters(unparsedRelRef[i], "(", ")", ","));
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
    vector<string> unparsedPatterns = splitClauseByAnds(wordList, curIndex, end, hasCorrectRelRefOrPatternForm);

    vector<tuple<string, vector<string>>> patternParams;
    for (const auto& unparsedPattern : unparsedPatterns) {
        patternParams.push_back(extractParameters(unparsedPattern, "(", ")", ","));
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

vector<ClauseType> SelectQueryParser::getAllClauseTypes() {
    return vector<ClauseType>{SELECT, SUCH_THAT, PATTERN, WITH};
}

vector<string> SelectQueryParser::splitClauseByAnds(vector<string>& wordList, int start, int end,
                                                    function<bool(string)> formChecker) {
    vector<int> ands = findAnds(wordList, start, end);
    vector<string> res;
    string condString = "";
    int curIndex = start;
    for (int i = 0; i < ands.size(); i++) {
        for (int j = curIndex; j < ands[i]; j++) {
            condString += wordList[j];
        }
        if (!formChecker(condString)) {
            curIndex = ands[i];
            continue;
        }
        res.push_back(condString);
        condString = "";
        curIndex = ands[i] + 1;
    }

    while (curIndex < end) {
        condString += wordList[curIndex];
        curIndex++;
    }
    if (!formChecker(condString)) {
        throw SyntaxException();
    }
    res.push_back(condString);
    return res;
}