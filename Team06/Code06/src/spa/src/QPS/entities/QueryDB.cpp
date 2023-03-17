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

vector<string> QueryDB::fetch(Parameter p) {
    for (Table t : tableVector) {
        if (t.hasParameter(p)) {
            return t.extractColumn(p);
        }
    }
    return {};
}