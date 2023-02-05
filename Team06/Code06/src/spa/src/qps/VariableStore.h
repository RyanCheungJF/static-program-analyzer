//
// Created by Faruq on 30/1/23.
//

#ifndef SPA_VARIABLESTORE_H
#define SPA_VARIABLESTORE_H
#include <string>
#include <unordered_map>
#include "entities/Parameter.h"
using namespace std;

class VariableStore {
public:
    VariableStore();
    ParameterType getType(Parameter);
    int insertVariable(Parameter);
    bool hasVariable(Parameter);
    bool updateSynonym(Parameter*);
    string toString();
//private:
    unordered_map<string, Parameter> store;
//    const static unordered_map<string, ParameterType> stringToTypeMap;
//    static ParameterType stringToType(string);
//    string value;
//    ParameterType type;
};


#endif //SPA_VARIABLESTORE_H
