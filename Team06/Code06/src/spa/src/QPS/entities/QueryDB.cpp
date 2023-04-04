#include "QueryDB.h"

QueryDB::QueryDB() {
    // Creates the QueryDB object
}

Table QueryDB::emptyTable = Table({}, {});

void QueryDB::insertTable(Table& table) {
    // Check if we have any duplicate parameters
    // if so do an intersection
    table.removeDuplicates();
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
            table.intersectTable(t);
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
    for (Parameter& param : params) {
        if (!this->hasParameter(param) && param.getType() != ParameterType::BOOLEAN) {
            vector<vector<string>> contentVec = {};
            vector<string> content = readPKB.findDesignEntities(param);
            for (string& c : content) {
                contentVec.push_back(std::move(vector<string>{c}));
            }
            Parameter p(param);
            p.updateAttributeType(AttributeType::NONE);
            Table table({p}, contentVec);
            this->insertTable(table);
        }
    }
    Table extracted = extractColumns(params, readPKB);
    return params[0].getType() == ParameterType::BOOLEAN ? hasEmptyTable() ? falseVec : trueVec
        : extracted.getResult(params);
}

bool QueryDB::hasEmptyTable() {
    for (const Table& t : tableVector) {
        if (t.isEmptyTable()) {
            return true;
        }
    }
    return false;
}

Table QueryDB::extractColumns(vector<Parameter> params, ReadPKB& readPKB) {
    // Assumes that each table has unique headers.
    // extracts in any order
    if (params[0].getType() == ParameterType::BOOLEAN || hasEmptyTable()) {
        return emptyTable;
    }
    vector<Table> temp;
    for (Table& table : tableVector) {
        vector<Parameter> headers = table.getHeaders();
        vector<Parameter> paramsVec;
        for (const Parameter& param : params) {
            if (find(headers.begin(), headers.end(), param) != headers.end()) {
                paramsVec.push_back(param);
            }
        }
        if (paramsVec.empty()) {
            continue;
        }
        table.extractColumns(paramsVec);
        temp.push_back(table);
    }
    for (Parameter& param : params) {
        if (param.hasAttribute()) {
            unordered_map<string, string> attributeMap;
            vector<vector<string>> mapping = readPKB.findAttribute(param);
            for (const vector<string>& kv : mapping) {
                attributeMap.insert({ kv[0], kv[1] });
            }
            for (Table& t : temp) {
                if (t.hasParameter(param)) {
                    t.updateValues(param, attributeMap);
                }
            }
        }
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