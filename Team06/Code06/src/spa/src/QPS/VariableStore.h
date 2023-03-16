//
// Created by Faruq on 30/1/23.
//

#ifndef SPA_VARIABLESTORE_H
#define SPA_VARIABLESTORE_H
#include <string>
#include <unordered_map>

#include "entities/Parameter.h"
#include "QPSGrammarUtils.h"
#include "exceptions/SemanticException.h"
using namespace std;

class VariableStore {
public:
    VariableStore();
    ParameterType getType(Parameter);
    void insertVariable(Parameter);
    bool hasVariable(Parameter);
    bool updateSynonym(Parameter*);
    string toString();
    unordered_map<string, Parameter> store;
};

#endif // SPA_VARIABLESTORE_H
