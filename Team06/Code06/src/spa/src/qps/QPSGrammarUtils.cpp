//
// Created by Faruq on 26/1/23.
//

#include "QPSGrammarUtils.h"
#include <iostream>
#include <string>
#include <regex>

bool isName(string s) {
    return regex_match(s, regex("^[a-zA-Z][a-zA-Z0-9]*$"));
}

bool isIdent(string s) {
    return isName(s);
}

bool isSynonym(string s) {
    return isIdent(s);
}

bool isInteger(string integer) {
    return regex_match(integer, regex("^[0-9]*$"));
}

bool isSelectClause(string selectClause) {
    return regex_match(selectClause, regex("^select"));
}

bool isSuchThatClause(string suchThatClause) {
    return regex_match(suchThatClause, regex("^such that"));
}

bool isSelect(string s) {
    return regex_match(s, regex("^select$"));
}

bool isPattern(string s) {
    return regex_match(s, regex("^pattern$"));
}

bool startsWithLetter(string s) {
    return regex_match(s, regex("^[a-zA-Z].*"));
}

bool isDeclaration(string declaration) {
    int index = declaration.find(" ");
    string declarationToken = declaration.substr(0, index);
    // The logic below will probably be used in parse declaration or something
//    string remainder = declaration.substr(index+1, declaration.length());
    return isDesignEntity(declarationToken);
}

bool isDesignEntity(string designEntity) {
    return regex_match(designEntity, regex("^(stmt|read|print|call|while|if|assign|variable|constant|procedure)"));
}
bool isStmtRef(string stmtRef) {
    return isSynonym(stmtRef) || isInteger(stmtRef) || (stmtRef == "_");
}

bool isEntRef(string entRef) {
    return isSynonym(entRef) || (entRef == "_") || regex_match(entRef, regex("^\"[a-zA-Z][a-zA-Z0-9]*\"$"));
}



//TODO: implement this, just do string matching, i.e. check if given string is the correct keyword
//bool QPSGrammarUtils::isFollows(string)
//bool QPSGrammarUtils::isParent(string)
//bool QPSGrammarUtils::isUses(string)
//bool QPSGrammarUtils::isModifies(string)

//TODO: Implement this
bool isExprSpec(string)
{
    return true;
}

bool isFollows(string s) {
    return true;
}

bool isParent(string s) {
    return true;
}

bool isUses(string s) {
    return true;
}

bool isModifies(string s) {
    return true;
}


bool isRelRef(string s) {
    return isFollows(s) || isParent(s) || isUses(s) || isModifies(s);
}