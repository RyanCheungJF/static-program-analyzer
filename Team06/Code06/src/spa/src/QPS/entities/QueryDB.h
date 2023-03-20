//
// Created by Faruq on 3/2/23.
//

#ifndef SPA_QUERYDB_H
#define SPA_QUERYDB_H
#include <string>
#include <vector>

#include "PKB/ReadPKB.h"
#include "Parameter.h"
#include "Table.h"

class QueryDB {
public:
    QueryDB();
    void insertTable(Table);
    vector<string> fetch(vector<Parameter>, ReadPKB&);
    vector<string> emptyVec = {};
    vector<string> falseVec = {"FALSE"};
    vector<string> trueVec = {"TRUE"};
    Table emptyTable = Table({}, {});

private:
    vector<Table> tableVector;
    Table extractColumns(vector<Parameter>);
    bool hasEmptyTable();
    bool hasParameter(Parameter&);
};

#endif // SPA_QUERYDB_H
