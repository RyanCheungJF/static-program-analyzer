#ifndef SPA_SELECTCLAUSEINFO_H
#define SPA_SELECTCLAUSEINFO_H
#include <qps/entities/Parameter.h>
using namespace std;

class SelectClauseInfo {
public:
	Parameter param;

	SelectClauseInfo(Parameter s);
	SelectClauseInfo(const SelectClauseInfo& sci);
};

#endif //SPA_SELECTCLAUSEINFO_H
