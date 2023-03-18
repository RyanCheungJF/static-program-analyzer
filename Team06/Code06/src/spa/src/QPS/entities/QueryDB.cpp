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
        }
        else {
            // intersect the two tables
            table = table.intersectTable(t);
        }
    }
    // This may push empty tables into the tableVector
    tableVector.push_back(table);
}

bool QueryDB::hasParameter(Parameter& p) {
    for (Table table : tableVector) {
        if (table.hasParameter(p)) {
            return true;
        }
    }
    return false;
}

vector<string> QueryDB::fetch(vector<Parameter> params, ReadPKB& readPKB) {
    vector<Parameter> presentParams;
    vector<Parameter> absentParams;
    Table initialTable = emptyTable;
    for (Parameter param : params) {
        if (this->hasParameter(param)) {
            presentParams.push_back(param);
        }
        else if (param.getType() != ParameterType::BOOLEAN) {
            vector<string> content = readPKB.findDesignEntities(param);
            vector<Parameter> paramVec = {param};
            vector<vector<string>> contentVec = {};
            for (string c : content) {
                contentVec.push_back({c});
            }
            Table table(paramVec, contentVec);
            if (initialTable.isEmptyTable()) {
                initialTable = table;
            }
            else {
                initialTable = initialTable.cartesianProduct(table);
            }
        }
    }
    if (!presentParams.empty()) {
        initialTable = initialTable.isEmptyTable()
                           ? extractColumns(presentParams)
                           : initialTable = extractColumns(presentParams).cartesianProduct(initialTable);
    }
    if (hasEmptyTable()) {
        initialTable = emptyTable;
    }
    return params[0].getType() == ParameterType::BOOLEAN ? hasEmptyTable() ? falseVec : trueVec
                                                         : initialTable.getResult();
}

bool QueryDB::hasEmptyTable() {
    for (Table t : tableVector) {
        if (t.isEmptyTable()) {
            return true;
        }
    }
    return false;
}

Table QueryDB::extractColumns(vector<Parameter> params) {
    // Assumes that each table has unique headers.
    vector<Table> temp;
    for (Table table : tableVector) {
        vector<Parameter> headers = table.getHeaders();
        vector<Parameter> paramsVec;
        for (Parameter param : headers) {
            if (find(params.begin(), params.end(), param) != params.end()) {
                paramsVec.push_back(param);
            }
        }
        if (paramsVec.empty()) {
            continue;
        }
        Table extracted = table.extractColumns(paramsVec);
        temp.push_back(extracted);
    }
    if (temp.size() == 1) {
        return temp[0];
    }
    else {
        Table t = temp[0];
        for (int i = 1; i < temp.size(); i++) {
            t.cartesianProduct(temp[i]);
        }
        return t;
    }
}