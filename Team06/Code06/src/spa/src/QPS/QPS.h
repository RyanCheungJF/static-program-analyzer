#ifndef SPA_QPS_H
#define SPA_QPS_H
#include "entities/Query.h"

#include "PKB/ReadPKB.h"
#include "QPS/parser/QPSParser.h"

class QPS {
public:
  vector<string> processQueries(string, ReadPKB &);
};

#endif // SPA_QPS_H
