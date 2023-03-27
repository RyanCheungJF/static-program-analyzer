#include "Table.h"

#include <utility>

Table::Table(vector<Parameter> headers, vector<vector<string>> contents) {
    vector<int> duplicateIndexes;
    this->headers = headers;
    this->contents = contents;
}

bool Table::hasParameter(const Parameter& p) {
    return find(headers.begin(), headers.end(), p) != headers.end();
}

const vector<Parameter>& Table::getHeaders() const {
    return headers;
}

const vector<vector<string>>& Table::getContent() const {
    return contents;
}

bool Table::hasIntersectingParams(Table t) {
    vector<Parameter> headers = t.getHeaders();
    for (const Parameter& p : headers) {
        if (this->hasParameter(p)) {
            return true;
        }
    }
    return false;
}

vector<pair<int, int>> Table::getIntersectingIndex(Table t1, Table t2) {
    vector<Parameter> h1 = t1.getHeaders();
    vector<Parameter> h2 = t2.getHeaders();
    vector<pair<int, int>> result;
    for (int i = 0; i < h1.size(); i++) {
        for (int j = 0; j < h2.size(); j++) {
            if (h1[i] == h2[j]) {
                result.emplace_back(i, j);
            }
        }
    }
    return result;
}

Table Table::cartesianProduct(Table table) {
    vector<Parameter> h1 = this->getHeaders();
    const vector<Parameter>& h2 = table.getHeaders();
    const vector<vector<string>>& c1 = this->getContent();
    const vector<vector<string>>& c2 = table.getContent();
    vector<vector<string>> c3;
    h1.insert(h1.end(), h2.begin(), h2.end());
    for (const vector<string>& row1 : c1) {
        for (const vector<string>& row2 : c2) {
            vector<string> dupRow(row1);
            dupRow.insert(dupRow.end(), row2.begin(), row2.end());
            c3.push_back(dupRow);
        }
    }
    return Table{h1, c3};
}

vector<vector<string>> Table::intersectContent(vector<vector<string>> c1, vector<vector<string>> c2,
                                               const vector<pair<int, int>>& intersectingIndexes) {
    unordered_multimap<string, int> hashmap;
    // we will use "value+value" as the key to the hashmap
    // depending on how many values in the intersectingIndex vector.
    for (int i = 0; i < c1.size(); i++) {
        string key;
        for (const pair<int, int>& intersectingIndex : intersectingIndexes) {
            if (key.empty()) {
                key += c1[i][intersectingIndex.first];
            }
            else {
                key += AppConstants::STRING_PLUS + c1[i][intersectingIndex.first];
            }
        }
        hashmap.insert({key, i});
    }
    vector<vector<string>> result;
    vector<int> indexesToRemove;
    for (const pair<int, int>& intersectingIndex : intersectingIndexes) {
        indexesToRemove.push_back(intersectingIndex.first);
    }
    for (int i = 0; i < c2.size(); i++) {
        string key;
        for (const pair<int, int>& intersectingIndex : intersectingIndexes) {
            if (key.empty()) {
                key += c2[i][intersectingIndex.second];
            }
            else {
                key += AppConstants::STRING_PLUS + c2[i][intersectingIndex.second];
            }
        }
        auto range = hashmap.equal_range(key);
        for (auto it = range.first; it != range.second; ++it) {
            vector<vector<string>>::value_type row;
            // Insert values to row
            row.insert(row.end(), c1[it->second].begin(), c1[it->second].end());
            row.insert(row.end(), c2[i].begin(), c2[i].end());
            for (int k = int(indexesToRemove.size()); k > 0; k--) {
                row.erase(row.begin() + indexesToRemove[k - 1]);
            }
            // Push the row
            result.push_back(row);
        }
    }
    return result;
}

vector<Parameter> Table::intersectHeader(vector<Parameter> h1, vector<Parameter> h2,
                                         const vector<pair<int, int>>& intersectingIndexes) {
    vector<Parameter> newHeader;
    for (int i = 0; i < h1.size(); i++) {
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
    vector<pair<int, int>> intersectingIndexes = getIntersectingIndex(*this, std::move(t));
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
    return extractColumns(indexes);
}

Table Table::updateValues(Parameter p, unordered_map<string, string> map) {
    int index;
    vector<vector<string>> newContents;
    for (int i = 0; i < headers.size(); i++) {
        if (headers[i] == p) {
            index = i;
        }
    }
    for (vector<string> row : contents) {
        row[index] = map[row[index]];
        newContents.push_back(row);
    }
    return Table{headers, newContents};
}

Table Table::extractColumns(vector<int>& indexes) {
    std::unordered_set<vector<string>, VectorStringHash> newContent;
    newContent.reserve(contents.size());
    vector<Parameter> newHeader;

    // I do not believe that there will be a case where the tables are empty.
    for (int index : indexes) {
        newHeader.push_back(headers[index]);
    }

    for (const vector<string>& entry : contents) {
        vector<string> newEntry;
        for (int index : indexes) {
            newEntry.push_back(entry[index]);
        }
        if (newContent.find(newEntry) == newContent.end()) {
            newContent.insert(newEntry);
        }
    }
    vector<vector<string>> newContentVec;
    newContentVec.reserve(newContent.size());
    for (auto& entry : newContent) {
        newContentVec.push_back(entry);
    }

    return Table{newHeader, newContentVec};
}

bool Table::isEmptyTable() const {
    return contents.empty();
}

Table Table::extractColumns(vector<Parameter> params) {
    // Assume that all the params called is confirmed to be present in the table
    vector<int> indexes;
    for (int i = 0; i < params.size(); i++) {
        for (int j = 0; j < headers.size(); j++) {
            if (params[i] == headers[j]) {
                indexes.push_back(j);
                break;
            }
        }
    }
    return extractColumns(indexes);
}

vector<string> Table::getResult(vector<Parameter> params) {
    vector<string> res;
    vector<int> indexOrder;
    for (Parameter param : params) {
        for (int i = 0; i < headers.size(); i++) {
            if (param == headers[i]) {
                indexOrder.push_back(i);
                break;
            }
        }
    }
    for (const vector<string>& stringVec : contents) {
        string row;
        for (int index : indexOrder) {
            row += stringVec[index];
            row += " ";
        }
        // remove the extra space.
        row.pop_back();
        res.push_back(row);
    }
    return res;
}