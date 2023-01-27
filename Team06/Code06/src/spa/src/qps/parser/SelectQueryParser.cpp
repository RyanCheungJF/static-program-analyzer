#include "SelectClauseParser.h"
#include "SelectQueryParser.h"

//assumes select clause is valid
//does not check if synonym is declared
SelectQueryInfo SelectClauseParser::parse(string selectQuery) {
	vector<vector<string>> clauses = getClauses(selectQuery);
	vector<string> selectClauses = clauses[0];
	vector<string> suchThatClauses = clauses[1];
	vector<string> patternClauses = clauses[2];
	vector<SelectClauseInfo> selectClauseInfos;
	vector<SuchThatInfo> suchThatInfos;
	vector<PatternInfo> patternInfos;

	for (string clause::selectClauses) {
		selectClauseInfos.push_back(parseSelectClause(clause));
	}

	for (string clause::suchThatClauses) {
		suchThatInfos.push_back(parseSuchThatClause(clause));
	}

	for (string clause::patternClauses) {
		patternInfos.push_back(parsePatternClause(clause));
	}

	SelectQueryInfo queryInfo;
	queryInfo.selectClauseInfos = selectClauseInfos;
	queryInfo.selectClauseInfos = suchThatInfos;
	queryInfo.patternInfos = patternInfos;
	return queryInfo;
}

/*
splits the select, such that, and pattern clauses

@returns: a vector of size 3, containing list of select clause, list of such that clause, and list of pattern clause in that order
*/
vector<vector<string>> SelectClauseParser::getClauses(string selectQuery) {
	//TODO: Implement this
	vector<vector<string>> res;
	return res;
}

SelectClauseInfo SelectClauseParser::parseSelectClause(string selectClause)
{
	return SelectClauseInfo();
}

SuchThatInfo SelectClauseParser::parseSuchThatClause(string suchThatClause)
{
	return SuchThatInfo();
}

PatternInfo SelectClauseParser::parsePatternClause(string patternClause)
{
	return PatternInfo();
}
