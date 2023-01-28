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

	SelectQueryInfo queryInfo;
	queryInfo.selectClauseInfos = selectClauseInfos;
	queryInfo.selectClauseInfos = suchThatInfos;
	queryInfo.patternInfos = patternInfos;
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

SelectClauseInfo SelectClauseParser::parseSelectClause(vector<string>& wordList, int start, int end)
{
	return SelectClauseInfo();
}

SuchThatInfo SelectClauseParser::parseSuchThatClause(vector<string>& wordList, int start, int end)
{
	return SuchThatInfo();
}

PatternInfo SelectClauseParser::parsePatternClause(vector<string>& wordList, int start, int end)
{
	return PatternInfo();
}

