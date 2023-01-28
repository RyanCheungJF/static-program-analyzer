#include "SuchThatInfo.h"

SuchThatInfo::SuchThatInfo() {
	relationship = "";
}

SuchThatInfo::SuchThatInfo(string rel, vector<string> ps)
{
	relationship = rel;
	params = ps;
}

SuchThatInfo::SuchThatInfo(const SuchThatInfo& sti) 
{
	relationship = sti.relationship;
	params = sti.params;
}
