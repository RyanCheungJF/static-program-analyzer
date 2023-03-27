#ifndef SPA_TABLE_H
#define SPA_TABLE_H
#include <string>
#include <unordered_map>
#include <vector>

#include "Parameter.h"

using namespace std;

struct VectorStringHash {
    size_t operator()(const std::vector<std::string>& v) const {
        std::hash<std::string> hasher;
        size_t seed = 0;
        for (std::string s : v) {
            seed ^= hasher(s) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

class Table {
public:
    Table(vector<Parameter>, vector<vector<string>>);
    Table extractDesignEntities();
    bool hasParameter(const Parameter&);
    const vector<Parameter>& getHeaders() const;
    const vector<vector<string>>& getContent() const;
    bool hasIntersectingParams(Table);
    Table intersectTable(Table);
    Table extractColumns(vector<int>&);
    Table extractColumns(vector<Parameter>);
    Table cartesianProduct(Table);
    vector<string> getResult(vector<Parameter>);
    bool isEmptyTable() const;
    Table updateValues(Parameter, unordered_map<string, string>);

private:
    vector<Parameter> headers;
    vector<vector<string>> contents;
    static vector<pair<int, int>> getIntersectingIndex(Table, Table);
    static vector<vector<string>> intersectContent(vector<vector<string>>, vector<vector<string>>,
                                                   const vector<pair<int, int>>&);
    static vector<Parameter> intersectHeader(vector<Parameter>, vector<Parameter>, const vector<pair<int, int>>&);
};

#endif // SPA_TABLE_H
