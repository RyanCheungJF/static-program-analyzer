#ifndef SPA_TABLE_H
#define SPA_TABLE_H
#include "Parameter.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Table {
public:
  // original implementation with vectors
//  Table(vector<Parameter> &, vector<vector<string>> &);
//  vector<Parameter> getHeaders();
//  vector<vector<string>> getContent();
//  Table selectColumns(vector<int> &);
//  vector<string> extractColumn(int);
//  Table extractDesignEntities();
//  vector<pair<int, int>> getAllIntersectingParams(Table);
//  pair<vector<int>, vector<int>> getAllNonIntersectingParams(Table);
//  Table intersectTables(Table, const vector<pair<int, int>> &intersection);

// implementation with unorderedMap
    bool hasParameter(const Parameter&);
    vector<Parameter> getHeaders();
    vector<Parameter> getIntersectingParams(Table);

private:
  // original implementation with vectors
//  vector<Parameter> headers;
//  vector<vector<string>> content;

// implementation using unorderedMap for headers with vector string for content.
  unordered_map<Parameter, vector<string>> table;
};

#endif // SPA_TABLE_H
