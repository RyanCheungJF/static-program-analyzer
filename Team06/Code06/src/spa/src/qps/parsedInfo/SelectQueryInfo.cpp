#include "SelectQueryInfo.h"

SelectQueryInfo::SelectQueryInfo(SelectClauseInfo sci, SuchThatInfo sti, PatternInfo pi) 
{
	selectClauseInfo = sci;
	suchThatInfo = sti;
	patternInfo = pi;
}
