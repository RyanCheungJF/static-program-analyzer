#include "QueryDB.h"

QueryDB::QueryDB() {
  // Creates the QueryDB object
}

void QueryDB::insertTable(Table table) {
  // Check if we have any duplicate parameters
  // if so do an intersection
  vector<Parameter> inputHeaders = table.getHeaders();
  vector<Parameter> seenParameters;
  for (Parameter &param : inputHeaders) {
    if (hasParameter(param)) {
      seenParameters.push_back(param);
    }
  }
  if (seenParameters.empty()) {
    tableVector.push_back(table);
  } else {
    // Throw everything in here in a function
    vector<Table> newTableVector;
    while (!tableVector.empty()) {
      Table t = tableVector.back();
      tableVector.pop_back();
      vector<pair<int, int>> intersection = getAllIntersectingParams(t, table);
      if (intersection.empty()) {
        // just insert the popped table into the newTableVector
        newTableVector.push_back(t);
      } else {
        // intersect the two tables
        table = intersectTables(t, table, intersection);
      }
    }
    newTableVector.push_back(table);
    tableVector = newTableVector;
  }
}

// TODO: this function should be placed in Table instead of QueryDB
Table QueryDB::intersectTables(Table t1, Table t2,
                               const vector<pair<int, int>> &intersection) {
  pair<vector<int>, vector<int>> nonIntersectionIndexes =
      getAllNonIntersectingParams(t1, t2);
  vector<Parameter> newHeader;
  vector<vector<string>> newContent;
  vector<Parameter> t1Head = t1.getHeaders();
  vector<Parameter> t2Head = t2.getHeaders();
  vector<vector<string>> t1Content = t1.getContent();
  vector<vector<string>> t2Content = t2.getContent();
  for (pair<int, int> index : intersection) {
    // Inserts the intersecting params first
    newHeader.push_back(t1Head[index.first]);
  }
  for (int index : nonIntersectionIndexes.first) {
    // Then inserts the remaining params for Table t
    newHeader.push_back(t1Head[index]);
  }
  for (int index : nonIntersectionIndexes.second) {
    // Then inserts the remaining params for Table table
    newHeader.push_back(t2Head[index]);
  }
  for (vector<string> firstContent : t1Content) {
    for (vector<string> secondContent : t2Content) {
      bool isMatch = true;
      for (pair<int, int> indexes : intersection) {
        if (firstContent[indexes.first] != secondContent[indexes.second]) {
          isMatch = false;
        }
      }
      if (isMatch) {
        vector<string> contentArr;
        for (pair<int, int> index : intersection) {
          // Inserts the intersecting params first
          contentArr.push_back(firstContent[index.first]);
        }
        for (int index : nonIntersectionIndexes.first) {
          // Then inserts the remaining params for Table t
          contentArr.push_back(firstContent[index]);
        }
        for (int index : nonIntersectionIndexes.second) {
          // Then inserts the remaining params for Table table
          contentArr.push_back(secondContent[index]);
        }
        newContent.push_back(contentArr);
      }
    }
  }
  return Table{newHeader, newContent};
}

vector<pair<int, int>> QueryDB::getAllIntersectingParams(Table t1, Table t2) {
  vector<Parameter> t1Head = t1.getHeaders();
  vector<Parameter> t2Head = t2.getHeaders();
  vector<pair<int, int>> intersection;
  for (int i = 0; i < t1Head.size(); i++) {
    for (int j = 0; j < t2Head.size(); j++) {
      if (t1Head[i].isEqualTo(t2Head[j])) {
        intersection.emplace_back(i, j);
      }
    }
  }
  return intersection;
}

pair<vector<int>, vector<int>> QueryDB::getAllNonIntersectingParams(Table t1,
                                                                    Table t2) {
  vector<pair<int, int>> intersection = getAllIntersectingParams(t1, t2);
  vector<Parameter> t1Head = t1.getHeaders();
  vector<Parameter> t2Head = t2.getHeaders();
  vector<int> t1Index;
  vector<int> t2Index;
  for (int i = 0; i < t1Head.size(); i++) {
    bool matched = false;
    for (pair<int, int> indexes : intersection) {
      if (i == indexes.first) {
        matched = true;
      }
    }
    if (!matched) {
      t1Index.push_back(i);
    }
  }
  for (int i = 0; i < t2Head.size(); i++) {
    bool matched = false;
    for (pair<int, int> indexes : intersection) {
      if (i == indexes.second) {
        matched = true;
      }
    }
    if (!matched) {
      t2Index.push_back(i);
    }
  }
  return pair{t1Index, t2Index};
}

bool QueryDB::hasParameter(Parameter &p) {
  for (Table table : tableVector) {
    vector<Parameter> headers = table.getHeaders();
    for (Parameter param : headers) {
      if (param.isEqualTo(p)) {
        return true;
      }
    }
  }
  return false;
}

vector<string> QueryDB::fetch(Parameter p) {
  int tableIndex;
  int columnIndex;
  for (int i = 0; i < tableVector.size(); i++) {
    vector<Parameter> headers = tableVector[i].getHeaders();
    for (int j = 0; j < headers.size(); j++) {
      if (headers[j].isEqualTo(p)) {
        tableIndex = i;
        columnIndex = j;
        break;
      }
    }
  }
  Table table = tableVector[tableIndex];
  return table.extractColumn(columnIndex);
}