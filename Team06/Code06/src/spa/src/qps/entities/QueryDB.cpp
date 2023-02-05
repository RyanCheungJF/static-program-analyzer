#include "QueryDB.h"

QueryDB::QueryDB() {
    // Creates the QueryDB object
}

QueryDB::QueryDB(Table table) {
    tableVector.push_back(table);
}

void QueryDB::insertTable(Table table) {
    // Check if we have any duplicate parameters
    // if so do an intersection
    vector<Parameter> inputHeaders = table.getHeaders();
    vector<Parameter> seenParameters;
    for(Parameter& param: inputHeaders) {
        if(hasParameter(param)) {
            seenParameters.push_back(param);
        }
    }
    if (seenParameters.empty()) {
        tableVector.push_back(table);
    } else {
        // do some form of intersection.
    }
}

bool QueryDB::hasParameter(Parameter& p) {
    for (Table table : tableVector) {
        vector<Parameter> headers = table.getHeaders();
        for(Parameter param: headers) {
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
    for(int i = 0; i < tableVector.size(); i++) {
        vector<Parameter> headers = tableVector[i].getHeaders();
        for(int j = 0; j < headers.size(); j++) {
            if(headers[j].isEqualTo(p)) {
                tableIndex = i;
                columnIndex = j;
                break;
            }
        }
    }
    Table table = tableVector[tableIndex];
    return table.extractColumn(columnIndex);
}