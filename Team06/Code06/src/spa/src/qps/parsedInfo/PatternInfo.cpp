#include "PatternInfo.h"

PatternInfo::PatternInfo()
{
	synAssign = "";
	entRef = "";
	pattern = "";
}

PatternInfo::PatternInfo(const PatternInfo& pi)
{
	synAssign = pi.synAssign;
	entRef = pi.entRef;
	pattern = pi.pattern;
}

PatternInfo::PatternInfo(string syn, string ent, string pat)
{
	synAssign = syn;
	entRef = ent;
	pattern = pat;
}
