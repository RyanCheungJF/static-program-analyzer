#include "Table.h"

#include <utility>

Table::Table(vector<Parameter> headers, vector<vector<string>> contents) {
    this->headers = std::move(headers);
    this->contents = std::move(contents);
}

bool Table::hasParameter(const Parameter& p) {
    return find(headers.begin(), headers.end(), p) != headers.end();
}

vector<Parameter> Table::getHeaders() {
    return headers;
}

vector<vector<string>> Table::getContent() {
    return contents;
}

bool Table::hasIntersectingParams(Table t) {
    for (const Parameter& p : t.getHeaders()) {
        if (this->hasParameter(p)) {
            return true;
        }
    }
    return false;
}

vector<pair<int,int>> Table::getIntersectingIndex(Table t1, Table t2) {
    vector<Parameter> h1 = t1.getHeaders();
    vector<Parameter> h2 = t2.getHeaders();
    vector<pair<int,int>> result;
    for (int i = 0; i < h1.size(); i++) {
        for (int j = 0; j < h2.size(); j++) {
            if (h1[i] == h2[j]) {
                result.emplace_back(i,j);
            }
        }
    }
    return result;
}

vector<string> Table::extractColumn(Parameter p) {
    int index;
    for(int i = 0; i < headers.size(); i++) {
        if (headers[i] == p) {
            index = i;
            break;
        }
    }
    vector<string> colVal;
    for (vector<string> entry : contents) {
        if (find(colVal.begin(), colVal.end(), entry[index]) == colVal.end()) {
            colVal.push_back(entry[index]);
        }
    }
    return colVal;
}

vector<vector<string>> Table::intersectContent(vector<vector<string>> c1, vector<vector<string>> c2, const vector<pair<int, int>>& intersectingIndexes) {
    unordered_multimap<string, int> hashmap;
    // we will use "value+value" as the key to the hashmap
    // depending on how many values in the intersectingIndex vector.
    for(int i = 0; i < c1.size(); i++) {
        string key;
        for(pair<int, int> intersectingIndex : intersectingIndexes) {
            if (key.empty()) {
                key += c1[i][intersectingIndex.first];
            } else {
                key += "+" + c1[i][intersectingIndex.first];
            }
        }
        hashmap.insert({key, i});
    }
    vector<vector<string>> result;
    vector<int> indexesToRemove;
    for (pair<int, int> intersectingIndex : intersectingIndexes) {
        indexesToRemove.push_back(intersectingIndex.first);
    }
    for (int i = 0; i < c2.size(); i++) {
        string key;
        for(pair<int, int> intersectingIndex : intersectingIndexes) {
            if (key.empty()) {
                key += c2[i][intersectingIndex.second];
            } else {
                key += "+" + c2[i][intersectingIndex.second];
            }
        }
        auto range = hashmap.equal_range(key);
        for (auto it = range.first; it != range.second; ++it) {
            vector<vector<string>>::value_type row;
            // Insert values to row
            row.insert(row.end(),c1[it->second].begin(),c1[it->second].end());
            row.insert(row.end(),c2[i].begin(),c2[i].end());
            for (int k = int(indexesToRemove.size()); k > 0; k--) {
                row.erase(row.begin() + indexesToRemove[k-1]);
            }
            // Push the row
            result.push_back(row);
        }
    }
    return result;
}

vector<Parameter> Table::intersectHeader(vector<Parameter> h1, vector<Parameter> h2, const vector<pair<int,int>>& intersectingIndexes) {
    vector<Parameter> newHeader;
    for(int i = 0; i < h1.size(); i++) {
        bool isIntersect = false;
        for (pair<int, int> inter : intersectingIndexes) {
            if (inter.first == i) {
                isIntersect = true;
            }
        }
        if (!isIntersect) {
            newHeader.push_back(h1[i]);
        }
    }
    newHeader.insert(newHeader.end(), h2.begin(), h2.end());
    return newHeader;
}


Table Table::intersectTable(Table t) {
    vector<vector<string>> c1 = contents;
    vector<vector<string>> c2 = t.getContent();
    vector<Parameter> h1 = headers;
    vector<Parameter> h2 = t.getHeaders();
    vector<pair<int,int>> intersectingIndexes = getIntersectingIndex(*this, std::move(t));
    vector<vector<string>> newContent = intersectContent(c1, c2, intersectingIndexes);
    vector<Parameter> newHeader = intersectHeader(h1, h2, intersectingIndexes);
    return Table{newHeader, newContent};
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

Table Table::selectColumns(vector<int> &indexes) {
    vector<vector<string>> newContent;
    vector<Parameter> newHeader;
    // I do not believe that there will be a case where the tables are empty.
    for (int index : indexes) {
        newHeader.push_back(headers[index]);
    }
    for (vector<string> entry : contents) {
        vector<string> newEntry;
        for (int index : indexes) {
            newEntry.push_back(entry[index]);
        }
        if (find(newContent.begin(), newContent.end(), newEntry) ==
            newContent.end()) {
            newContent.push_back(newEntry);
        }
    }
    return Table{newHeader, newContent};
}