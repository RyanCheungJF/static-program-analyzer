//
// Created by Faruq on 26/1/23.
//

#ifndef SPA_QPSGRAMMARUTILS_H
#define SPA_QPSGRAMMARUTILS_H
#include <string>

#include "QPS/parser/ParserUtil.h"
using namespace std;

// No need to package it into a class

bool isName(string);
bool isIdent(string);
bool isSynonym(string);
bool isPattern(string);
bool startsWithLetter(string);
bool isSelect(string);
bool isInteger(string);
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
bool isFixedStringWithWildCard(string);
bool hasCorrectRelRefOrPatternForm(string);
bool isElem(string);
bool isAttrRef(string);

#endif // SPA_QPSGRAMMARUTILS_H
