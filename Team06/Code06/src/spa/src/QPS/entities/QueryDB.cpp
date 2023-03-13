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
    for (Parameter param : absentParams) {
        vector<string> content = readPKB.findDesignEntities(param);
        vector<Parameter> paramVec = {param};
        vector<vector<string>> contentVec = {content};
        Table table(paramVec, contentVec);
        // TODO: waste of time. this can be improved.
        insertTable(table);
    }
    return extractColumns(params);
//  int tableIndex;
//  int columnIndex;
//  for (int i = 0; i < tableVector.size(); i++) {
//    vector<Parameter> headers = tableVector[i].getHeaders();
//    for (int j = 0; j < headers.size(); j++) {
//      if (headers[j] == p) {
//        tableIndex = i;
//        columnIndex = j;
//        break;
//      }
//    }
//  }
//  Table table = tableVector[tableIndex];
//  return table.extractColumn(columnIndex);
}

vector<string> QueryDB::extractColumns(vector<Parameter> params) {

    for (Table table : tableVector) {
        vector<Parameter> headers = table.getHeaders();

    }
}