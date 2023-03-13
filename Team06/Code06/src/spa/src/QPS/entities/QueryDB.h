//
// Created by Faruq on 3/2/23.
//

#ifndef SPA_QUERYDB_H
#define SPA_QUERYDB_H
#include "PKB/ReadPKB.h"
#include "Parameter.h"
#include "Table.h"
#include <string>
#include <vector>

class QueryDB {
public:
  QueryDB();
  void insertTable(Table);
  bool hasParameter(Parameter &);
  vector<string> fetch(vector<Parameter>, ReadPKB&);

private:
  vector<Table> tableVector;
  vector<string> extractColumns(vector<Parameter>);
};

#endif // SPA_QUERYDB_H
