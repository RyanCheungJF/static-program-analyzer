#include "QueryDB.h"

QueryDB::QueryDB() {
  // Creates the QueryDB object
}

void QueryDB::insertTable(Table table) {
  // Check if we have any duplicate parameters
  // if so do an intersection
  vector<Parameter> inputHeaders = table.getHeaders();
  vector<Parameter> seenParameters;
  vector<Table> oldTableVector = tableVector;
  tableVector.clear();
  while (!oldTableVector.empty()) {
    Table t = oldTableVector.back();
      oldTableVector.pop_back();
    if (!table.hasIntersectingParams(t)) {
      // just insert the popped table into the newTableVector
        tableVector.push_back(t);
    } else {
      // intersect the two tables
      table = table.intersectTable(t);
    }
  }
  tableVector.push_back(table);
}


bool QueryDB::hasParameter(Parameter &p) {
  for (Table table : tableVector) {
    if (table.hasParameter(p)) {
      return true;
    }
  }
  return false;
}

vector<string> QueryDB::fetch(vector<Parameter> params, ReadPKB &readPKB) {
    // It is assumed here that none of the Tables in tableVector
    // is an empty table. Any empty tables should not have been inserted.
    vector<Parameter> presentParams;
    vector<Parameter> absentParams;
    vector<Table> tempStore;
    for (Parameter param : params) {
        if (this->hasParameter(param)) {
            presentParams.push_back(param);
        } else {
            vector<string> content = readPKB.findDesignEntities(param);
            vector<Parameter> paramVec = {param};
            vector<vector<string>> contentVec = {};
            for (string c : content) {
                contentVec.push_back({c});
            }
            Table table(paramVec, contentVec);
            tempStore.push_back(table);
        }
    }
    Table initialTable({}, {{}});
    if (presentParams.empty()) {
        initialTable = tempStore[0];
        tempStore.erase(tempStore.begin());
    } else {
        initialTable = extractColumns(presentParams);
    }
    for (Table t : tempStore) {
        initialTable = initialTable.cartesianProduct(t);
    }
    return initialTable.getResult();
}

Table QueryDB::extractColumns(vector<Parameter> params) {
    // Assumes that each table has unique headers.
    assert(!params.empty());
    vector<Table> temp;
    for (Table table : tableVector) {
        vector<Parameter> headers = table.getHeaders();
        vector<Parameter> paramsVec;
        for (Parameter param : headers) {
            if (find(params.begin(),params.end(),param) != params.end()) {
                paramsVec.push_back(param);
            }
        }
        Table extracted = table.extractColumns(paramsVec);
        temp.push_back(extracted);
    }
    if (temp.size() == 1) {
        return temp[0];
    } else {
        Table t = temp[0];
        for (int i = 1; i < temp.size(); i++) {
            t.cartesianProduct(temp[i]);
        }
        return t;
    }
}