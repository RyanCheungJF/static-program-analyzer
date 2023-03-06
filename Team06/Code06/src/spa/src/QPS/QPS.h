#ifndef SPA_QPS_H
#define SPA_QPS_H
#include "PKB/ReadPKB.h"
#include "QPS/parser/QPSParser.h"
#include "entities/Query.h"

class QPS {
public:
    vector<string> processQueries(string, ReadPKB&);
};

#endif // SPA_QPS_H
