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
  vector<string> fetch(vector<Parameter>, ReadPKB&);

private:
  vector<Table> tableVector;
  Table extractColumns(vector<Parameter>);
  bool hasParameter(Parameter&);
};

#endif // SPA_QUERYDB_H
