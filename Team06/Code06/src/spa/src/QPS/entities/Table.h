#ifndef SPA_TABLE_H
#define SPA_TABLE_H
#include "Parameter.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Table {
public:
    Table(vector<Parameter>, vector<vector<string>>);
    Table extractDesignEntities();
    Table selectColumns(vector<int> &);
    bool hasParameter(const Parameter&);
    vector<Parameter> getHeaders();
    vector<vector<string>> getContent();
    vector<Parameter> getIntersectingParams(Table);
    Table intersectTable(Table, const vector<Parameter>&);
    vector<string> extractColumn(Parameter);

private:
    vector<Parameter> headers;
    vector<vector<string>> contents;
    static vector<pair<int, int>> getIntersectingIndex(Table, Table);
    static vector<vector<string>> intersectContent(vector<vector<string>>,vector<vector<string>>,const vector<pair<int,int>>&);
    static vector<Parameter> intersectHeader(vector<Parameter>, vector<Parameter>, const vector<pair<int, int>>&);
};

#endif // SPA_TABLE_H
