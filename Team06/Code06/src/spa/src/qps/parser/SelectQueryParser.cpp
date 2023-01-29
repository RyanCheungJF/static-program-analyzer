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
	Parameter param("synonym", wordList[1]);
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
		cout << "Exception encountered: " << e;
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

	vector<string> params{ param1, param2 };

	return SuchThatInfo(rel, params);
}



PatternInfo SelectClauseParser::parsePatternClause(vector<string>& wordList, int start, int end)
{
	//logic is similar to suchThatParser, not sure how to extract both out
	if (start == -1 && end == -1) {
		return PatternInfo();
	}
	if (end <= start) {
		throw Exception();
	}
	if (end - start < 3) {
		throw Exception();
	}

	stringstream ss;
	int condStart = start + 1;
	while (int i = condStart; i < end; i++) {
		ss << wordList[i];
	}
	string condString = ss.str();

	string assignSyn;
	string entRef;
	string pattern;
	string delimiter = "(";
	size_t itemStart = 0;
	size_t itemEnd;

	try {
		tie(assignSyn, itemEnd) = extractSubStringUntilDelimiter(condString, itemStart, delimiter);
		itemStart = itemEnd;
		delimiter = ",";
		tie(entRef, itemEnd) = extractSubStringUntilDelimiter(condString, itemStart, delimiter);
		itemStart = itemEnd;
		delimiter = ")";
		tie(pattern, itemEnd) = extractSubStringUntilDelimiter(condString, itemStart, delimiter);
	}
	catch (Exception e) {
		cout << "Exception encountered: " << e;
	}

	if (!isSynonym(assignSyn)) {
		throw Exception();
	}

	if (!isEntRef(entRef)) {
		throw Exception();
	}

	if (!isExprSpec(pattern)) {
		throw Exception();
	}

	if (itemEnd != condString.size()) {
		throw Exception();
	}

	return PatternInfo(assignSyn, entRef, pattern);
}

