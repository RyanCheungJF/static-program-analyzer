//
// Created by Faruq on 30/1/23.
//
#include <iostream>
#include "VariableStore.h"

VariableStore::VariableStore() {
    // Some constructor method.

}

ParameterType VariableStore::getType(Parameter synonym) {
    return store[synonym.getValue()].getType();
}

int VariableStore::insertVariable(Parameter p) {
    if (!hasVariable(p)) {
        store[p.getValue()] = p;
        return 1;
    } else {
        //TODO: throw some error
    }
    return 0;
}

bool VariableStore::hasVariable(Parameter p) {
    return !(store.find(p.getValue()) == store.end());
}

bool VariableStore::updateSynonym(Parameter* synP)
{
    if (!hasVariable(*synP)) {
        return false;
    }
    ParameterType synType = getType(*synP);
    synP->updateSynonymType(synType);
    return true;
}

string VariableStore::toString() {
    string s;
    for(const pair<const string, Parameter> elem : store) {
        s += "value: " + elem.first + ", type: " + elem.second.typeToString(elem.second.getType()) + "\n";
    }
    return s;
}

//ParameterType VariableStore::getType(string) {
////    return Parameter::Parameter().getType();
//    return undefined;
//}