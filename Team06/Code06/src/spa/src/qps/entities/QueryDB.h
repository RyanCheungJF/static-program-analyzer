//
// Created by Faruq on 3/2/23.
//

#ifndef SPA_QUERYDB_H
#define SPA_QUERYDB_H
#include "Parameter.h"
#include "Table.h"
#include <string>
#include <vector>

class QueryDB {
public:
    QueryDB();
    QueryDB(Table);
    void insertTable(Table);
    bool hasParameter(Parameter&);
    vector<string> fetch(Parameter);
private:
    vector<Table> tableVector;
    vector<pair<int, int>> getAllIntersectingParams(Table, Table);
    pair<vector<int>, vector<int>> getAllNonIntersectingParams(Table, Table);
};


#endif //SPA_QUERYDB_H
