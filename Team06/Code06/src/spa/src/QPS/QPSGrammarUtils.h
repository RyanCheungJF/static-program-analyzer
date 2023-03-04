//
// Created by Faruq on 26/1/23.
//

#ifndef SPA_QPSGRAMMARUTILS_H
#define SPA_QPSGRAMMARUTILS_H
#include "QPS/parser/ParserUtil.h"
#include <string>
using namespace std;

// No need to package it into a class

bool isName(string);
bool isIdent(string);
bool isSynonym(string);
bool isPattern(string);
bool startsWithLetter(string);
bool isSelect(string);
bool isRelRef(string);
bool isInteger(string);
// bool isSelectClause(string);
// bool isSuchThatClause(string);
bool isDeclaration(string);
bool isDesignEntity(string);
pair<string, string> extractDesignEntity(string);
bool isWildCard(string);
bool isStmtRef(string);
bool isFixedString(string);
bool isEntRef(string);
bool isExprSpec(string);
bool isTerm(string);
bool isExpr(string);
bool isFactor(string);
bool isFollows(string);
bool isParent(string);
bool isUses(string);
bool isModifies(string);
bool isFixedStringWithWildCard(string);
bool hasCorrectRelRefOrPatternForm(string);

#endif // SPA_QPSGRAMMARUTILS_H
