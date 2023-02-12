#include "SelectQueryParser.h"

//assumes select clause is valid
//does not check if synonym is declared
Query SelectQueryParser::parse(string selectQuery) {
	vector<string> wordList = stringToWordList(selectQuery);
	vector<int> clauseStarts = getClauseStarts(wordList);
	vector<int> clauseEnds = getClauseEnds(clauseStarts, wordList.size());

	vector<Parameter> selectParams = parseSelectClause(wordList, clauseStarts[0], clauseEnds[0]);
	vector<Relationship> suchThatRelations = parseSuchThatClause(wordList, clauseStarts[1], clauseEnds[1]);
	vector<Pattern> patterns = parsePatternClause(wordList, clauseStarts[2], clauseEnds[2]);

	Query query(selectParams, suchThatRelations, patterns);
	return query;
}

/*
splits the select, such that, and pattern clauses, 
assuming we only have at most one of each type of clauses 

@returns: a vector of size 3, containing list of select clause, list of such that clause, and list of pattern clause in that order
*/
vector<int> SelectQueryParser::getClauseStarts(vector<string> &wordList) {
	//TODO: Implement this
	int suchThatStart = findSuchThat(wordList);
	int patternStart = findPattern(wordList);
	int selectStart = 0;
	vector<int> res{ selectStart, suchThatStart, patternStart };
	return res;
}

/*

*/
vector<int> SelectQueryParser::getClauseEnds(vector<int> clauseStarts, int wordListLength)
{
	map<int, int> initialIndices;
	vector<int> res(clauseStarts.size(), -1);
	int clauseEnd;
	for (int i = 0; i < clauseStarts.size(); i++) {
		if (clauseStarts[i] == -1) {
			continue;
		}
		initialIndices.insert({ clauseStarts[i], i });
	}
	sort(clauseStarts.begin(), clauseStarts.end());
	for (int i = 0; i < clauseStarts.size(); i++) {
		if (clauseStarts[i] == -1) {
			continue;
		}
		clauseEnd = (i != clauseStarts.size() - 1) ? clauseStarts[i + (size_t) 1] : wordListLength;
		res[initialIndices[clauseStarts[i]]] = clauseEnd;
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
		throw Exception();
	}
	if (!isSelect(wordList[start])) {
		throw Exception();
	}
	if (!isSynonym(wordList[end - (size_t) 1])) {
		throw Exception();
	}
	//TODO: replace with synonym type rather than string
	Parameter param(wordList[1], "synonym");
	params.push_back(param);
	return params;
}

/*
Currently capable of parsing one condition after such that, with 2 params
use loops for extensibility
*/
vector<Relationship> SelectQueryParser::parseSuchThatClause(vector<string>& wordList, int start, int end)
{
	vector<Relationship> res;
	if (start == -1 && end == -1) {
		return res;
	}
	if (end <= start) {
		throw Exception();
	}
	if (end - start < 3) {
		throw Exception();
	}

	stringstream ss;
	int condStart = start + 2;
	for (int i = condStart; i < end; i++) {
		ss << wordList[i];
	}
	string condString = ss.str();

	string rel;
	string param1;
	string param2;
	string delimiter = "(";
	size_t itemStart = 0;
	size_t itemEnd;
	try {
		tie(rel, itemEnd) = extractSubStringUntilDelimiter(condString, itemStart, delimiter);
		itemStart = itemEnd;
		delimiter = ",";
		tie(param1, itemEnd) = extractSubStringUntilDelimiter(condString, itemStart, delimiter);
		itemStart = itemEnd;
		delimiter = ")";
		tie(param2, itemEnd) = extractSubStringUntilDelimiter(condString, itemStart, delimiter);
	}
	catch (Exception e) {
		//do nothing for now
	}

	if (!isRelRef(rel)) {
		throw Exception();
	}

	//TODO: parse relationship first, and from there we can decide on how to filter params
	//or it could be done while creating the relationship object
	if (!isStmtRef(param1) && !isEntRef(param1)) {
		throw Exception();
	}

	if (!isStmtRef(param2) && !isEntRef(param2)) {
		throw Exception();
	}

	if (itemEnd != condString.size()) {
		throw Exception();
	}
	Parameter p1(removeCharFromString(param1, '\"'), Parameter::guessParameterType(param1));
	Parameter p2(removeCharFromString(param2, '\"'), Parameter::guessParameterType(param2));
	vector<Parameter> params{ p1, p2 };
	//need to parse params first
	Relationship relationship = Relationship::makeRelationship(rel, params);
	res.push_back(relationship);


	return res;
}


//TODO fix bug with patternclause
vector<Pattern> SelectQueryParser::parsePatternClause(vector<string>& wordList, int start, int end)
{
	vector<Pattern> res;
	if (start == -1 && end == -1) {
		return res;
	}
	if (end <= start) {
		throw Exception();
	}
	if (end - start < 2) {
		throw Exception();
	}

	stringstream ss;
	int condStart = start + 1;
	for (int i = condStart; i < end; i++) {
		ss << wordList[i];
	}
	string condString = ss.str();
	vector<tuple<string, string, string>> patternParams = extractParameters(condString);
	for (tuple<string, string, string> t : patternParams) {
		string synAssignString, entRefString, patternString;
		tie(synAssignString, entRefString, patternString) = t;
		if (!isSynonym(synAssignString)) {
			throw Exception();
		}

		if (!isEntRef(entRefString)) {
			throw Exception();
		}

		if (!isExprSpec(patternString)) {
			throw Exception();
		}
		Parameter synAssign(synAssignString, ParameterType::SYNONYM);
		Parameter entRef(removeCharFromString(entRefString, '\"'), Parameter::guessParameterType(entRefString));
		Pattern p(synAssign, entRef, removeCharFromString(patternString, '\"'));
		res.push_back(p);
	}

	return res;
}

