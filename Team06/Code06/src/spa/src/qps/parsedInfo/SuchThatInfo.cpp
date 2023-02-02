#include "SuchThatInfo.h"

SuchThatInfo::SuchThatInfo() {
}

SuchThatInfo::SuchThatInfo(Relationship r)
{
	rel = r;
}

SuchThatInfo::SuchThatInfo(const SuchThatInfo& sti) 
{
	rel = sti.rel;
}
