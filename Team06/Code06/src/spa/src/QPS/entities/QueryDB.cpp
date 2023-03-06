#include "QueryDB.h"

QueryDB::QueryDB() {
  // Creates the QueryDB object
}

void QueryDB::insertTable(Table table) {
  // Check if we have any duplicate parameters
  // if so do an intersection
  vector<Parameter> inputHeaders = table.getHeaders();
  vector<Parameter> seenParameters;
  vector<Table> newTableVector;
  while (!tableVector->empty()) {
    Table t = tableVector->back();
    tableVector->pop_back();
    vector<Parameter> intersection = table.getIntersectingParams(t);
    if (intersection.empty()) {
      // just insert the popped table into the newTableVector
      newTableVector.push_back(t);
    } else {
      // intersect the two tables
      table = table.intersectTables(t, intersection);
    }
  }
  newTableVector.push_back(table);
  tableVector = &newTableVector;
}

// TODO: this function should be placed in Table instead of QueryDB


bool QueryDB::hasParameter(Parameter &p) {
  for (Table table : *tableVector) {
    if (table.hasParameter(p)) {
      return true;
    }
  }
  return false;
}

vector<string> QueryDB::fetch(Parameter p) {
  int tableIndex;
  int columnIndex;
  for (int i = 0; i < tableVector->size(); i++) {
    vector<Parameter> headers = (*tableVector)[i].getHeaders();
    for (int j = 0; j < headers.size(); j++) {
      if (headers[j] == p) {
        tableIndex = i;
        columnIndex = j;
        break;
      }
    }
  }
  Table table = (*tableVector)[tableIndex];
  return table.extractColumn(columnIndex);
}