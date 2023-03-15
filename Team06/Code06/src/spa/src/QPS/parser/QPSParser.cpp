//
// Created by Faruq on 31/1/23.
//
#include "QPSParser.h"

#include "../QPSGrammarUtils.h"

QPSParser::QPSParser() {}

vector<Query> QPSParser::parse(string qpsQuery) {
    // split the code
    vector<string> queryStatements = splitQuery(qpsQuery);
    SelectQueryParser selectQueryParser;
    vector<Query> queryVec;
    VariableStore vStore;
    vector<string> declarations;
    for (string queryStatement : queryStatements) {
        if (isDeclaration(queryStatement)) {
            declarations.push_back(queryStatement);
        }
        else if (isSelect(queryStatement)) {
            Query query = selectQueryParser.parse(queryStatement);
            queryVec.push_back(query);
        }
        else {
            throw SyntaxException();
        }
    }
    if (queryVec.empty()) {
        // no select query
        throw SyntaxException();
    }
    vStore = parseDeclarations(declarations);
    // need to do it this way cos dealing with pointer
    for (int i = 0; i < queryVec.size(); i++) {
        checkSynonyms(&queryVec.at(i), vStore);
    }

    return queryVec;
}

vector<string> QPSParser::splitQuery(string qpsQuery) {
    qpsQuery = trim(qpsQuery);
    // Check if the last term is a semicolon.
    if (qpsQuery.back() == ';') {
        throw SyntaxException();
    }
    string delimiter = ";";
    vector<string> clauses;
    int start = 0;
    while (start != -1) {
        if (start >= qpsQuery.length()) {
            throw SyntaxException();
        }
        string clause;
        bool found;
        tie(clause, start, found) =
            extractSubStringUntilDelimiter(qpsQuery, start, delimiter);
        clause = trim(clause);
        clauses.push_back(clause);
    }
    return clauses;
}

void QPSParser::checkSynonyms(Query* query, VariableStore varStore) {
    vector<Parameter*> synPs = query->getAllUncheckedSynonyms();
    for (int i = 0; i < synPs.size(); i++) {
        if (!varStore.updateSynonym(synPs.at(i))) {
            // undeclared synonyms
            throw SemanticException();
        }
    }
    if (!query->validateAllParameters()) {
        // invalid parameter types
        throw SemanticException();
    }
}
