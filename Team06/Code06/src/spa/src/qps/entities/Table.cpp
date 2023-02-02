//
// Created by Faruq on 3/2/23.
//

#include "Table.h"

Table::Table(vector<Parameter>& headers, vector<vector<string>>& content) {
    this->headers = headers;
    this->content = content;
}

void Table::intersect(Table otherTable) {
    // do some shit
    // table x otherTable;
};

vector<Parameter> Table::getHeaders() {
    return headers;
}

Table Table::selectColumns(vector<int>& indexes) {
    vector<vector<string>> newTable;
    vector<Parameter> newHeader;
    for(int index: indexes) {
        newHeader.push_back(headers[index]);
    }
    for(vector<string> entry: content) {
        vector<string> newEntry;
        for(int index: indexes) {
            newEntry.push_back(entry[index]);
        }
        newTable.push_back(newEntry);
    }
    // No fucking clue what the fuck is braced initialization.
    return Table{newHeader, newTable};
}

vector<string> Table::extractColumn(int index) {
    vector<string> colVal;
    for(vector<string> entry: content) {
        colVal.push_back(entry[index]);
    }
    return colVal;
}

Table Table::extractDesignEntities() {
    vector<int> indexes;
    for(int i = 0; i < headers.size(); i++) {
        if (Parameter::isDsgEntity(headers[i])) {
            indexes.push_back(i);
        }
    }
    return selectColumns(indexes);
}