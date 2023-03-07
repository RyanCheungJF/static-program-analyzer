//
// Created by Faruq on 3/2/23.
//

#include "Table.h"

Table::Table(vector<Parameter>& headers, vector<vector<string>>& content) {
    this->headers = headers;
    this->content = content;
}

vector<Parameter> Table::getHeaders() {
    return headers;
}

vector<vector<string>> Table::getContent() {
    return content;
}

Table Table::selectColumns(vector<int>& indexes) {
    vector<vector<string>> newContent;
    vector<Parameter> newHeader;
    // I do not believe that there will be a case where the tables are empty.
    for (int index : indexes) {
        newHeader.push_back(headers[index]);
    }
    for (vector<string> entry : content) {
        vector<string> newEntry;
        for (int index : indexes) {
            newEntry.push_back(entry[index]);
        }
        if (find(newContent.begin(), newContent.end(), newEntry) == newContent.end()) {
            newContent.push_back(newEntry);
        }
    }
    // No fucking clue what the fuck is braced initialization.
    return Table{newHeader, newContent};
}

vector<string> Table::extractColumn(int index) {
    vector<string> colVal;
    for (vector<string> entry : content) {
        if (find(colVal.begin(), colVal.end(), entry[index]) == colVal.end()) {
            colVal.push_back(entry[index]);
        }
    }
    return colVal;
}

Table Table::extractDesignEntities() {
    vector<int> indexes;
    for (int i = 0; i < headers.size(); i++) {
        if (Parameter::isDsgEntity(headers[i])) {
            indexes.push_back(i);
        }
    }
    return selectColumns(indexes);
}