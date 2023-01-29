#include "SelectClauseInfo.h"

SelectClauseInfo::SelectClauseInfo(Parameter s)
{
	param = s;
}

SelectClauseInfo::SelectClauseInfo(const SelectClauseInfo& sci)
{
	param = sci.param;
}
