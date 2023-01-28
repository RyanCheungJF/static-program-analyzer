//
// Created by Faruq on 26/1/23.
//

#include "QPSGrammarUtils.h"
#include <iostream>
#include <string>
#include <regex>

bool QPSGrammarUtils::isName(string name) {
    return regex_match(name, regex("^[a-zA-Z][a-zA-Z0-9]*$"));
}

bool QPSGrammarUtils::isIdent(string ident) {
    return isName(ident);
}

bool QPSGrammarUtils::isSynonym(string synonym) {
    return isIdent(synonym);
}

bool QPSGrammarUtils::isInteger(string integer) {
    return regex_match(integer, regex("^[0-9]*$"));
}

bool QPSGrammarUtils::isSelectClause(string selectClause) {
    return regex_match(selectClause, regex("^select"));
}

bool QPSGrammarUtils::isSuchThatClause(string suchThatClause) {
    return regex_match(suchThatClause, regex("^such that"));
}

bool isPattern(string s) {
    return regex_match(s, regex("^pattern$"));
}

bool startsWithLetter(string s) {
    return regex_match(s, regex("^[a-zA-Z].*"));
}

bool QPSGrammarUtils::isDeclaration(string declaration) {
    int index = declaration.find(" ");
    string declarationToken = declaration.substr(0, index);
    // The logic below will probably be used in parse declaration or something
//    string remainder = declaration.substr(index+1, declaration.length());
    return isDesignEntity(declarationToken);
}

bool QPSGrammarUtils::isDesignEntity(string designEntity) {
    return regex_match(designEntity, regex("^(stmt|read|print|call|while|if|assign|variable|constant|procedure)"));
}
bool QPSGrammarUtils::isStmtRef(string stmtRef) {
    return isSynonym(stmtRef) || isInteger(stmtRef) || (stmtRef == "_");
}

bool QPSGrammarUtils::isEntRef(string entRef) {
    return isSynonym(entRef) || (entRef == "_") || regex_match(entRef, regex("^\"[a-zA-Z][a-zA-Z0-9]*\"$"));
}

bool QPSGrammarUtils::isRelRef(string relRef) {
    return isFollows(relRef) || isParent(relRef) || isUses(relRef) || isModifies(relRef);
}
// NOT SURE IF WE WANT TO IMPLEMENT THIS HERE
//bool QPSGrammarUtils::isFollows(string)
//bool QPSGrammarUtils::isParent(string)
//bool QPSGrammarUtils::isUses(string)
//bool QPSGrammarUtils::isModifies(string)