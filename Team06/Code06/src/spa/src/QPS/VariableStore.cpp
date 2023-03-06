//
// Created by Faruq on 30/1/23.
//
#include "VariableStore.h"

VariableStore::VariableStore() = default;

ParameterType VariableStore::getType(Parameter synonym) {
    return store[synonym.getValue()].getType();
}

void VariableStore::insertVariable(Parameter p) {
    if (!hasVariable(p)) {
        store[p.getValue()] = p;
    }
    else {
        throw SemanticException();
    }
}

bool VariableStore::hasVariable(Parameter p) {
    return !(store.find(p.getValue()) == store.end());
}

bool VariableStore::updateSynonym(Parameter* synP) {
    if (!hasVariable(*synP)) {
        return false;
    }
    ParameterType synType = getType(*synP);
    synP->updateSynonymType(synType);
    return true;
}

string VariableStore::toString() {
    string s;
    for (const pair<const string, Parameter>& elem : store) {
        s += "value: " + elem.first + ", type: " + elem.second.getTypeString() + "\n";
    }
    return s;
}