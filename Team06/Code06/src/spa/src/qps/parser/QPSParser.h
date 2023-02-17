//
// Created by Faruq on 31/1/23.
//

#ifndef SPA_QPSPARSER_H
#define SPA_QPSPARSER_H
#include "DeclarationParser.h"
#include "ParserUtil.h"
#include "SelectQueryParser.h"
#include "qps/entities/Query.h"
#include "exceptions/SemanticException.h"
using namespace std;

class QPSParser {
public:
    QPSParser();
    vector<Query> parse(string);
//private:
    vector<string> splitQuery(string);
    void checkSynonyms(Query*, VariableStore);
};


#endif //SPA_QPSPARSER_H
