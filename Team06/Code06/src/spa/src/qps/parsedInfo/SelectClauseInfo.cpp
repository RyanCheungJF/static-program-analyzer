#include "SelectClauseInfo.h"

SelectClauseInfo::SelectClauseInfo(Synonym s)
{
	synonym = s;
}

SelectClauseInfo::SelectClauseInfo(SelectClauseInfo& sci)
{
	synonym = sci.synonym;
}
