//
// Created by Faruq on 27/1/23.
//

#ifndef SPA_DECLARATIONPARSER_H
#define SPA_DECLARATIONPARSER_H
#include <map>
#include <string>
#include "../VariableStore.h"
using namespace std;



VariableStore parseDeclarations(vector<string>);
vector<Parameter> declarationToParameter(string);


#endif //SPA_DECLARATIONPARSER_H
