//
// Created by Faruq on 3/2/23.
//

#ifndef SPA_TABLE_H
#define SPA_TABLE_H
#include <string>
#include <vector>

#include "Parameter.h"

using namespace std;

class Table {
public:
    Table(vector<Parameter>&, vector<vector<string>>&);
    vector<Parameter> getHeaders();
    vector<vector<string>> getContent();
    Table selectColumns(vector<int>&);
    vector<string> extractColumn(int);
    Table extractDesignEntities();

private:
    vector<Parameter> headers;
    vector<vector<string>> content;
};

#endif // SPA_TABLE_H
