#include "SelectClauseInfo.h"

SelectClauseInfo::SelectClauseInfo()
{
	param = Parameter();
}

SelectClauseInfo::SelectClauseInfo(Parameter s)
{
	param = s;
}

SelectClauseInfo::SelectClauseInfo(const SelectClauseInfo& sci)
{
	param = sci.param;
}