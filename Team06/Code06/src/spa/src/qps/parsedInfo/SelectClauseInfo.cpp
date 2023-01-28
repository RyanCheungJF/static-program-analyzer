#include "SelectClauseInfo.h"

SelectClauseInfo::SelectClauseInfo(Synonym s)
{
	synonym = s;
}

SelectClauseInfo::SelectClauseInfo(const SelectClauseInfo& sci)
{
	synonym = sci.synonym;
}
