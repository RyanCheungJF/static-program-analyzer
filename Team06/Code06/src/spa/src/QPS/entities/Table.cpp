//
// Created by Faruq on 3/2/23.
//

#include "Table.h"

Table::Table(vector<Parameter> &headers, vector<vector<string>> &content) {
  this->headers = headers;
  this->content = content;
}

vector<Parameter> Table::getHeaders() { return headers; }

vector<vector<string>> Table::getContent() { return content; }

Table Table::selectColumns(vector<int> &indexes) {
  vector<vector<string>> newContent;
  vector<Parameter> newHeader;
  // I do not believe that there will be a case where the tables are empty.
  for (int index : indexes) {
    newHeader.push_back(headers[index]);
  }
  for (vector<string> entry : content) {
    vector<string> newEntry;
    for (int index : indexes) {
      newEntry.push_back(entry[index]);
    }
    if (find(newContent.begin(), newContent.end(), newEntry) ==
        newContent.end()) {
      newContent.push_back(newEntry);
    }
  }
  return Table{newHeader, newContent};
}

Table Table::intersectTables(Table t, const vector<pair<int, int>> &intersection) {
    pair<vector<int>, vector<int>> nonIntersectionIndexes = getAllNonIntersectingParams(t);
    vector<Parameter> newHeader;
    vector<vector<string>> newContent;
    vector<Parameter> t1Head = this->getHeaders();
    vector<Parameter> t2Head = t.getHeaders();
    vector<vector<string>> t1Content = this->getContent();
    vector<vector<string>> t2Content = t.getContent();
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

vector<pair<int, int>> Table::getAllIntersectingParams(Table t) {
    vector<Parameter> t1Head = this->getHeaders();
    vector<Parameter> t2Head = t.getHeaders();
    vector<pair<int, int>> intersection;
    for (int i = 0; i < t1Head.size(); i++) {
        for (int j = 0; j < t2Head.size(); j++) {
            if (t1Head[i] == t2Head[j]) {
                intersection.emplace_back(i, j);
            }
        }
    }
    return intersection;
}

pair<vector<int>, vector<int>> Table::getAllNonIntersectingParams(Table t) {
    vector<pair<int, int>> intersection = getAllIntersectingParams(t);
    vector<Parameter> t1Head = this->getHeaders();
    vector<Parameter> t2Head = t.getHeaders();
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

vector<string> Table::extractColumn(int index) {
  vector<string> colVal;
  for (vector<string> entry : content) {
    if (find(colVal.begin(), colVal.end(), entry[index]) == colVal.end()) {
      colVal.push_back(entry[index]);
    }
  }
  return colVal;
}

Table Table::extractDesignEntities() {
  vector<int> indexes;
  for (int i = 0; i < headers.size(); i++) {
    if (Parameter::isDsgEntity(headers[i])) {
      indexes.push_back(i);
    }
  }
  return selectColumns(indexes);
}