#include "DeclarationParser.h"

using namespace std;

vector<Parameter> declarationToParameter(string s) {
    // The string should look something like "stmt s1, s2" at this point
    vector<Parameter> paramList;
    string designEntity, synonyms;
    tie(designEntity, synonyms) = extractDesignEntity(s);
    vector<string> synonymList = stringToWordListByDelimiter(synonyms, ",");
    for (const string& synonym : synonymList) {
        if (!isSynonym(synonym)) {
            throw SyntaxException();
        }
        Parameter p = Parameter::makeParameter(synonym);
        ParameterType pt = Parameter::stringToType(designEntity);
        p.updateSynonymType(pt);
        paramList.push_back(p);
    }
    return paramList;
}

VariableStore parseDeclarations(vector<string> declarations) {
    // The string should look something like "stmt s1, s2; call cl; assign a1,
    // a2;" at this point
    VariableStore store = VariableStore();
    for (string declaration : declarations) {
        // extract each declaration and insert it to the store.
        vector<Parameter> paramList = declarationToParameter(declaration);
        for (Parameter parameter : paramList) {
            store.insertVariable(parameter);
        }
    }
    return store;
}