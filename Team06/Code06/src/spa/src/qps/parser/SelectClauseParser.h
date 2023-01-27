#ifndef SPA_SELECTCLAUSEPARSER_H
#define SPA_SELECTCLAUSEPARSER_H
#include "Parser.h"
#include "qps/parsedInfo/SelectClauseInfo.h"
#include <string>
using namespace std;

class SelectClauseParser : public Parser<SelectClauseInfo> {
public:
	SelectClauseInfo parse(string item);
};

#endif //SPA_SELECTCLAUSEPARSER_H