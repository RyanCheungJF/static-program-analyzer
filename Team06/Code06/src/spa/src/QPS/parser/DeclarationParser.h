//
// Created by Faruq on 27/1/23.
//

#ifndef SPA_DECLARATIONPARSER_H
#define SPA_DECLARATIONPARSER_H
#include <list>
#include <map>
#include <string>

#include "../QPSGrammarUtils.h"
#include "../VariableStore.h"
#include "ParserUtil.h"
#include "exceptions/SyntaxException.h"
using namespace std;

VariableStore parseDeclarations(vector<string>);
vector<Parameter> declarationToParameter(string);

#endif // SPA_DECLARATIONPARSER_H
