#ifndef SPA_TABLE_H
#define SPA_TABLE_H
#include <string>
#include <unordered_map>
#include <vector>

#include "Parameter.h"

using namespace std;

class Table {
public:
    Table(vector<Parameter>, vector<vector<string>>);
    Table extractDesignEntities();
    bool hasParameter(const Parameter&);
    vector<Parameter> getHeaders();
    vector<vector<string>> getContent();
    bool hasIntersectingParams(Table);
    Table intersectTable(Table);
    Table extractColumns(vector<int>&);
    Table extractColumns(vector<Parameter>);
    Table cartesianProduct(Table);
    vector<string> getResult();
    bool isEmptyTable();

private:
    vector<Parameter> headers;
    vector<vector<string>> contents;
    static vector<pair<int, int>> getIntersectingIndex(Table, Table);
    static vector<vector<string>> intersectContent(vector<vector<string>>, vector<vector<string>>,
                                                   const vector<pair<int, int>>&);
    static vector<Parameter> intersectHeader(vector<Parameter>, vector<Parameter>, const vector<pair<int, int>>&);
};

#endif // SPA_TABLE_H
