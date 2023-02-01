//
// Created by Faruq on 31/1/23.
//

#ifndef SPA_QPSPARSER_H
#define SPA_QPSPARSER_H
#include "DeclarationParser.h"
#include "ParserUtil.h"
#include "SelectQueryParser.h"
#include "../entities/Query.h"


class QPSParser {
public:
    QPSParser();
    vector<Query> parse(string);
private:
    vector<string> splitQuery(string);
    void checkParameters(Query, VariableStore);
};


#endif //SPA_QPSPARSER_H
