#ifndef SPA_SELECTCLAUSEINFO_H
#define SPA_SELECTCLAUSEINFO_H
#include <qps/entities/Synonym.h>
using namespace std;

class SelectClauseInfo {
public:
	Synonym synonym;

	SelectClauseInfo(Synonym s);
	SelectClauseInfo(SelectClauseInfo& sci);
};

#endif //SPA_SELECTCLAUSEINFO_H
