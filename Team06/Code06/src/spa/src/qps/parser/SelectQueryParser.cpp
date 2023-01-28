#include "SelectQueryParser.h"

//assumes select clause is valid
//does not check if synonym is declared
SelectQueryInfo SelectClauseParser::parse(string selectQuery) {
	vector<string> wordList = stringToWordList(selectQuery);
	vector<int> clauseStarts = getClauseStarts(wordList);
	vector<int> clauseEnds = getClauseEnds(clauseStarts, wordList.size());

	SelectClauseInfo selectClauseInfo;
	SuchThatInfo suchThatInfo;
	PatternInfo patternInfo;

	selectClauseInfo = parseSelectClause(wordList, clauseStarts[0], clauseStarts[0]);
	suchThatInfo = parseSuchThatClause(wordList, clauseStarts[1], clauseStarts[1]);
	patternInfo = parsePatternClause(wordList, clauseStarts[2], clauseStarts[2]);

	SelectQueryInfo queryInfo(selectClauseInfo, suchThatInfo, patternInfo);
	return queryInfo;
}

/*
splits the select, such that, and pattern clauses, 
assuming we only have at most one of each type of clauses 

@returns: a vector of size 3, containing list of select clause, list of such that clause, and list of pattern clause in that order
*/
vector<int> SelectClauseParser::getClauseStarts(vector<string> &wordList) {
	//TODO: Implement this
	int suchThatStart = findSuchThat(wordList);
	int patternStart = findPattern(wordList);
	int selectStart = 0;
	vector<int> res{selectStart, suchThatStart, patternStart}
	return res;
}

/*

*/
vector<int> SelectClauseParser::getClauseEnds(vector<int> clauseStarts, int wordListLength)
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
		clauseEnd = (i != clauseStarts.size() - 1) ? clauseStarts[i + 1] : wordListLength;
		res[initialIndices[clauseStarts[i]]] = clauseEnd;
	}

	return res;
}

/*
assumes start and end won't be -1 i.e. select clause must exist
*/
SelectClauseInfo SelectClauseParser::parseSelectClause(vector<string>& wordList, int start, int end)
{
	if (end - start != 2) {
		throw Exception();
	}
	if (!isSelect(wordList[start])) {
		throw Exception();
	}
	if (!isSynonym(wordList[end - 1])) {
		throw Exception();
	}
	//TODO: replace with synonym type rather than string
	Synonym syn("unknown", wordList[1]);
	return SelectClauseInfo(syn);
}

/*
Currently capable of parsing one condition after such that, with 2 params
*/
SuchThatInfo SelectClauseParser::parseSuchThatClause(vector<string>& wordList, int start, int end)
{
	if (start == -1 && end == -1) {
		return SuchThatInfo();
	}
	if (end <= start) {
		throw Exception();
	}
	if (end - start < 3) {
		throw Exception();
	}

	stringstream ss;
	int condStart = start + 2;
	while (int i = condStart; i < end; i++) {
		ss << wordList[i];
	}
	string condString = ss.str();

	//TODO: extract this out into a util function
	//Find relref
	size_t itemStart = 0;
	size_t itemEnd = condString.find("(");
	if (itemEnd == string::npos) {
		throw Exception();
	}
	string rel = condString.substr(itemStart, itemEnd);
	if (!isRelRef(rel)) {
		throw Exception();
	}
	//Find 1st param
	itemStart = itemEnd;
	itemEnd = condString.find(",", itemStart);
	if (itemEnd == string::npos) {
		throw Exception();
	}
	string param1 = condString.substr(itemStart, itemEnd);
	//TODO: parse relationship first, and from there we can decide on how to filter params
	//or it could be done while creating the relationship object
	if (!isStmtRef(param1) && !isEntRef(param1)) {
		throw Exception();
	}
	//Find 2nd param
	itemStart = itemEnd;
	itemEnd = condString.find(")", itemStart);
	if (itemEnd == string::npos) {
		throw Exception();
	}
	string param2 = condString.substr(itemStart, itemEnd);
	if (!isStmtRef(param2) && !isEntRef(param2)) {
		throw Exception();
	}
	//Check closing bracket is last char
	if (itemEnd != condString.size() - 1) {
		throw Exception();
	}

	vector<string> params{ param1, param2 };

	return SuchThatInfo(rel, params);
}



PatternInfo SelectClauseParser::parsePatternClause(vector<string>& wordList, int start, int end)
{
	//logic is similar to suchThatParser
	if (start == -1 && end == -1) {
		return SuchThatInfo();
	}
	if (end <= start) {
		throw Exception();
	}
	if (end - start < 3) {
		throw Exception();
	}
	return PatternInfo();
}

