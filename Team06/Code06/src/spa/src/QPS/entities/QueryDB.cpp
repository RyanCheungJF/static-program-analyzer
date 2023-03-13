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
    vector<Parameter> presentParams;
    vector<Parameter> absentParams;
    for (Parameter param : params) {
        if (this->hasParameter(param)) {
            presentParams.push_back(param);
        } else {
            absentParams.push_back(param);
        }
    }
    Table extracted = extractColumns(presentParams);
    for (Parameter param : absentParams) {
        vector<string> content = readPKB.findDesignEntities(param);
        vector<Parameter> paramVec = {param};
        vector<vector<string>> contentVec = {content};
        Table table(paramVec, contentVec);
        extracted.cartesianProduct(table);
    }
    return extracted.getResult();
}

Table QueryDB::extractColumns(vector<Parameter> params) {
    // Assumes that each table has unique headers.
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