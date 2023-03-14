//
// Created by Faruq on 3/2/23.
//

#ifndef SPA_QUERYDB_H
#define SPA_QUERYDB_H
#include <string>
#include <vector>

#include "Parameter.h"
#include "Table.h"

class QueryDB {
public:
    QueryDB();
    void insertTable(Table);
    bool hasParameter(Parameter&);
    vector<string> fetch(Parameter);

private:
    vector<Table> tableVector;
};

#endif // SPA_QUERYDB_H
