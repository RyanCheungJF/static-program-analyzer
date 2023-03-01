#include "SelectQueryParser.h"

//assumes select clause is valid
//does not check if synonym is declared
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

@returns: a vector of size 3, containing list of select clause, list of such that clause, and list of pattern clause in that order
*/
map<ClauseType, vector<int>> SelectQueryParser::getClauseStarts(vector<string> &wordList) {
	vector<int> suchThatStart = findSuchThat(wordList);
	vector<int> patternStart = findPattern(wordList);
	vector<int> withStart{};
	vector<int> selectStart{ 0 };


	map<ClauseType, vector<int>> res{ { SELECT, selectStart}, {SUCH_THAT, suchThatStart} , {PATTERN, patternStart}, {WITH, withStart} };
	return res;
}

vector<tuple<ClauseType, int, int>> SelectQueryParser::getClausePositions(map<ClauseType, vector<int>> clauseStarts, int wordListLength)
{
	vector<ClauseType> allClauseTypes = getAllClauseTypes();
	map<int, ClauseType> startToClauseType;
	vector<int> clauseStartsVector;
	for (ClauseType ct : allClauseTypes) {
		vector<int> starts = clauseStarts[ct];
		for (int s : starts) {
			clauseStartsVector.push_back(s);
			startToClauseType.insert({ s, ct });
		}
	}

	sort(clauseStartsVector.begin(), clauseStartsVector.end());
	vector<tuple<ClauseType, int, int>> res;
	for (int i = 0; i < clauseStartsVector.size(); i++) {
		int clauseStart = clauseStartsVector[i];
		int clauseEnd = (i != clauseStartsVector.size() - 1) ? clauseStartsVector[i + (size_t)1] : wordListLength;
		ClauseType ct = startToClauseType[clauseStart];
		res.push_back({ ct, clauseStart, clauseEnd });
	}

	return res;
}

/*
assumes start and end won't be -1 i.e. select clause must exist
*/
vector<Parameter> SelectQueryParser::parseSelectClause(vector<string>& wordList, int start, int end)
{
	vector<Parameter> params;
	if (end - start != 2) {
		//select clause does not exist
		throw SyntaxException();
	}
	if (!isSelect(wordList[start])) {
		throw InternalException("Error: SelectQueryParser.parseSelectClause bad start position for wordList");
	}
	if (!isSynonym(wordList[end - (size_t) 1])) {
		//bad select parameter
		throw SyntaxException();
	}
	//TODO: replace with synonym type rather than string
	Parameter param(wordList[1], AppConstants::SYNONYM);
	params.push_back(param);
	return params;
}

/*
Currently capable of parsing one condition after such that, with 2 params
use loops for extensibility
*/
vector<shared_ptr<Relationship>> SelectQueryParser::parseSuchThatClause(vector<string>& wordList, int start, int end)
{
	vector<shared_ptr<Relationship>> res;
	if (start == -1 && end == -1) {
		return res;
	}
	if (end <= start) {
		throw InternalException("Error: SelectQueryParser.parseSuchThatClause bad start position and end position");
	}
	if (end - start < 3) {
		throw SyntaxException();
	}

	int curIndex = start + 2;

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
	unparsedRelRef.push_back(condString);

	vector<tuple<string, string, string, string>> relRefParams;
	for (int i = 0; i < unparsedRelRef.size(); i++) {
		relRefParams.push_back(extractParameters(unparsedRelRef[i]));
	}

	for (tuple<string, string, string, string> t : relRefParams) {
		string rel, param1, param2;
		tie(rel, param1, param2, std::ignore) = t;

		Parameter p1(removeCharFromString(param1, '\"'), Parameter::guessParameterType(param1));
		Parameter p2(removeCharFromString(param2, '\"'), Parameter::guessParameterType(param2));
		vector<Parameter> params{ p1, p2 };
		res.push_back(Relationship::makeRelationship(rel, params));
	}

	return res;
}

vector<Pattern> SelectQueryParser::parsePatternClause(vector<string>& wordList, int start, int end)
{
	vector<Pattern> res;
	if (start == -1 && end == -1) {
		return res;
	}
	if (end <= start) {
		throw InternalException("Error: SelectQueryParser.parsePatternClause bad start position and end position");
	}
	if (end - start < 2) {
		throw SyntaxException();
	}

	int curIndex = start + 1;

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
	unparsedPatterns.push_back(condString);


	vector<tuple<string, string, string, string>> patternParams;
	for (const auto & unparsedPattern : unparsedPatterns) {
		patternParams.push_back(extractParameters(unparsedPattern));
	}

	for (tuple<string, string, string, string> t : patternParams) {
		string patternDsgEntString, entRefString, patternString, ifsString;
		tie(patternDsgEntString, entRefString, patternString, ifsString) = t;
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
		Parameter entRef(removeCharFromString(entRefString, '\"'), Parameter::guessParameterType(entRefString));
        Parameter ifParam("", ParameterType::UNKNOWN);
        if(!ifsString.empty()) {
            ifParam = *new Parameter("", ParameterType::WILDCARD);
        }
        res.emplace_back(patternDsgEnt, entRef, patternString, ifParam);
	}
	return res;
}

vector<ClauseType> SelectQueryParser::getAllClauseTypes()
{
	return vector<ClauseType>{SELECT, SUCH_THAT, PATTERN, WITH};
}

