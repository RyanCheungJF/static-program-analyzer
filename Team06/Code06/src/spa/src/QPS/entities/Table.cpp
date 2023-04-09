#include "Table.h"

#include <utility>

Table::Table(vector<Parameter> headers, vector<vector<string>> contents) {
    vector<int> duplicateIndexes;
    this->headers = std::move(headers);
    this->contents = std::move(contents);
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

bool Table::hasIntersectingParams(Table& t) {
    const vector<Parameter>& h2 = t.getHeaders();
    for (const Parameter& p : h2) {
        if (this->hasParameter(p)) {
            return true;
        }
    }
    return false;
}

vector<pair<int, int>> Table::getIntersectingIndex(Table& t2) {
    vector<Parameter> h2 = t2.getHeaders();
    vector<pair<int, int>> result;
    for (int i = 0; i < headers.size(); i++) {
        for (int j = 0; j < h2.size(); j++) {
            if (headers[i] == h2[j]) {
                result.emplace_back(i, j);
            }
        }
    }
    return result;
}

void Table::cartesianProduct(Table& table) {
    const vector<Parameter>& h2 = table.getHeaders();
    const vector<vector<string>>& c1 = this->getContent();
    const vector<vector<string>>& c2 = table.getContent();
    vector<vector<string>> c3;
    headers.insert(headers.end(), h2.begin(), h2.end());
    for (const vector<string>& row1 : c1) {
        for (const vector<string>& row2 : c2) {
            // duplicates the row
            vector<string> dupRow(row1);
            dupRow.insert(dupRow.end(), row2.begin(), row2.end());
            c3.push_back(dupRow);
        }
    }
    contents = c3;
}

vector<vector<string>> Table::intersectContent(const vector<vector<string>>& c1, const vector<vector<string>>& c2,
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

vector<Parameter> Table::intersectHeader(const vector<Parameter>& h1, const vector<Parameter>& h2,
                                         const vector<pair<int, int>>& intersectingIndexes) {
    vector<Parameter> newHeader;
    for (int i = 0; i < h1.size(); i++) {
        bool isIntersect = false;
        for (pair<int, int> inter : intersectingIndexes) {
            if (inter.first == i) {
                isIntersect = true;
                break;
            }
        }
        if (!isIntersect) {
            newHeader.push_back(h1[i]);
        }
    }
    newHeader.insert(newHeader.end(), h2.begin(), h2.end());
    return newHeader;
}

void Table::intersectTable(Table& t) {
    const vector<vector<string>>& c1 = contents;
    const vector<vector<string>>& c2 = t.getContent();
    const vector<Parameter>& h1 = headers;
    const vector<Parameter>& h2 = t.getHeaders();
    vector<pair<int, int>> intersectingIndexes = this->getIntersectingIndex(t);
    vector<vector<string>> newContent = intersectContent(c1, c2, intersectingIndexes);
    vector<Parameter> newHeader = intersectHeader(h1, h2, intersectingIndexes);
    headers = newHeader;
    contents = newContent;
}

void Table::extractDesignEntities() {
    vector<int> indexes;
    for (int i = 0; i < headers.size(); i++) {
        if (headers[i].isDsgEntity()) {
            indexes.push_back(i);
        }
    }
    this->extractColumns(indexes);
}

void Table::updateValues(Parameter& p, unordered_map<string, string>& map) {
    int index = 0;
    for (int i = 0; i < headers.size(); i++) {
        if (headers[i] == p && !headers[i].hasAttribute()) {
            headers[i].updateAttributeType(p.getAttribute());
            index = i;
            break;
        }
    }
    for (vector<string>& row : contents) {
        row[index] = map[row[index]];
    }
}

void Table::extractColumns(vector<int>& indexes) {
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
    this->headers = newHeader;
    this->contents = newContentVec;
}

bool Table::isEmptyTable() const {
    return contents.empty();
}

void Table::extractColumns(vector<Parameter>& params) {
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
    extractColumns(indexes);
}

void Table::removeDuplicates() {
    vector<int> duplicateIndexes;
    vector<Parameter> newHeader = headers;
    headers.clear();
    for (int i = 0; i < newHeader.size(); i++) {
        if (!hasParameter(newHeader[i])) {
            this->headers.push_back(newHeader[i]);
        }
        else {
            duplicateIndexes.push_back(i);
        }
    }
    if (!duplicateIndexes.empty()) {
        vector<vector<string>> newContent = contents;
        contents.clear();
        for (vector<string> content : newContent) {
            for (int index : duplicateIndexes) {
                content.erase(content.begin() + index);
            }
            this->contents.push_back(content);
        }
    }
}

vector<string> Table::getResult(vector<Parameter>& params) {
    vector<string> res;
    vector<int> indexOrder;
    for (Parameter& param : params) {
        for (int i = 0; i < headers.size(); i++) {
            if (param == headers[i] && param.getAttribute() == headers[i].getAttribute()) {
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