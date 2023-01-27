#ifndef SPA_SELECTCLAUSEINFO_H
#define SPA_SELECTCLAUSEINFO_H
#include <qps/entities/Synonym.h>
#include <SuchThatInfo.h>
#include <PatternInfo.h>
using namespace std;

class SelectClauseInfo {
public:
	Synonym s;
	SuchThatInfo* suchThatInfo;
	PatternInfo* patternInfo;
};

#endif //SPA_SELECTCLAUSEINFO_H
