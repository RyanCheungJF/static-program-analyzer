//
// Created by Faruq on 26/1/23.
//

#include "QPSGrammarUtils.h"

#include <algorithm>
#include <regex>
#include <string>

#include "utils/AppConstants.h"

bool isName(string s) {
    return regex_match(trim(s), regex("^[a-zA-Z][a-zA-Z0-9]*$"));
}

bool isIdent(string s) {
    return isName(s);
}

bool isSynonym(string s) {
    return isIdent(s);
}

bool isTupleStart(string s) {
    return s[0] == '<';
}

bool isBoolean(string s) {
    return s == AppConstants::BOOLEAN;
}

bool isInteger(string integer) {
    return regex_match(trim(integer), regex("^0$|^[1-9][0-9]*$"));
}

bool isSelect(string s) {
    return regex_search(trim(s), regex("^Select"));
}

bool isPattern(string s) {
    return regex_match(trim(s), regex("^pattern$"));
}

bool startsWithLetter(string s) {
    return regex_match(trim(s), regex("^[a-zA-Z].*"));
}

bool hasBalancedBrackets(string s) {
    int balance = 0;
    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c == '(') {
            balance += 1;
        }
        else if (c == ')') {
            balance -= 1;
        }
        if (balance < 0) {
            return false;
        }
    }
    return balance == 0;
}

bool hasCorrectRelRefOrPatternForm(string s) {
    bool regexMatched = regex_match(s, regex("^[a-zA-Z].*\(.*\)$"));
    bool bracketBalanced = hasBalancedBrackets(s);
    return regexMatched && bracketBalanced;
}

bool isDeclaration(string declaration) {
    int index = declaration.find(" ");
    string declarationToken = declaration.substr(0, index);
    return isDesignEntity(declarationToken);
}

bool isDesignEntity(string designEntity) {
    return regex_search(trim(designEntity), regex("^(stmt|read|print|call|while|if|assign|variable|"
                                            "constant|procedure)"));
}

pair<string, string> extractDesignEntity(string designEntity) {
    regex rgx("^(stmt|read|print|call|while|if|assign|variable|constant|"
              "procedure)\\s+");
    smatch match;
    string remainder;
    string trimmedString = trim(designEntity);
    if (regex_search(trimmedString, match, rgx)) {
        remainder = match.suffix().str();
    }
    return pair(match[1], remainder);
}

bool isFixedString(string s) {
    s = trim(s);
    if (s.size() < 2) {
        return false;
    }
    if (s.at(0) != '\"') {
        return false;
    }
    if (s.at(s.size() - 1) != '\"') {
        return false;
    }
    return isSynonym(s.substr(1, s.size() - 2));
}

bool isWildCard(string s) {
    return trim(s) == "_";
}

bool isStmtRef(string stmtRef) {
    return isSynonym(stmtRef) || isInteger(stmtRef) || isWildCard(stmtRef);
}

bool isEntRef(string entRef) {
    return isSynonym(entRef) || isWildCard(entRef) || isFixedString(entRef);
}

bool isExprSpec(string s) {
    s = trim(s);
    if (s.empty()) {
        return false;
    }
    if (s == "_") {
        return true;
    }
    bool startsWith_ = regex_search(s, regex("^_\""));
    bool endsWith_ = regex_search(s, regex("\"_$"));
    if (startsWith_ && endsWith_) {
        if (s.size() < 5) {
            return false;
        }
        // This will get rid of _" and "_
        // If s = _"X+Y"_ then expr = X+Y
        string expr = s.substr(2, s.size() - 4);
        return isExpr(expr);
    }
    bool startsWithQuotation = regex_search(s, regex("^\""));
    bool endsWithQuotation = regex_search(s, regex("\"$"));
    if (startsWithQuotation && endsWithQuotation) {
        if (s.size() < 3) {
            return false;
        }
        // This will get rid of _" and "_
        // If s = _"X+Y"_ then expr = X+Y
        string expr = s.substr(1, s.size() - 2);
        return isExpr(expr);
    }
    return false;
}

bool isExpr(string s) {
    if (s.empty()) {
        return false;
    }
    int index = -1;
    int bracketsCounter = 0;
    // finds the first + or - from the back where there are no brackets
    // encapsulating them.
    for (int i = s.size() - 1; i >= 0; i--) {
        if ((s[i] == '+' || s[i] == '-') && bracketsCounter == 0) {
            index = i;
            break;
        }
        if (s[i] == '(') {
            bracketsCounter--;
        }
        else if (s[i] == ')') {
            bracketsCounter++;
        }
    }
    if (index == -1) {
        // regex search not found
        return isTerm(s);
    }
    if (index == s.size() - 1 || index == 0) {
        // cannot have + or - at start or end of string
        return false;
    }
    string first = trim(s.substr(0, index));
    string second = trim(s.substr(index + 1, s.size() - 1 - index));
    return isExpr(first) && isTerm(second);
}

bool isTerm(string s) {
    if (s.empty()) {
        return false;
    }
    // finds the first */% from the back where there are no brackets encapsulating
    // them.
    int index = -1;
    int bracketsCounter = 0;
    for (int i = s.size() - 1; i >= 0; i--) {
        if ((s[i] == '*' || s[i] == '/' || s[i] == '%') && bracketsCounter == 0) {
            index = i;
            break;
        }
        if (s[i] == '(') {
            bracketsCounter--;
        }
        else if (s[i] == ')') {
            bracketsCounter++;
        }
    }
    if (index == -1) {
        // None match found
        return isFactor(s);
    }
    if (index == s.size() - 1 || index == 0) {
        // operator cannot be at start and end of string
        return false;
    }
    string first = trim(s.substr(0, index));
    string second = trim(s.substr(index + 1, s.size() - 1 - index));
    return isTerm(first) && isFactor(second);
}

bool isFactor(string s) {
    if (s.empty()) {
        return false;
    }
    // recursively remove brackets
    bool hasBrackets = false;
    if (s[0] == '(' && s[s.size() - 1] == ')') {
        hasBrackets = true;
        s = s.substr(1, s.size() - 2);
    }
    if (hasBrackets) {
        return isExpr(s);
    }
    return isName(s) || isInteger(s);
}

bool isElem(string s) {
    return isSynonym(trim(s)) || isAttrRef(trim(s));
}

bool isAttrRef(string s) {
    string delimiter = ".";
    bool found;
    int nextStart;
    string name, attribute;
    tie(name, nextStart, found) = extractSubStringUntilDelimiter(s, 0, delimiter);
    if (!found) {
        return false;
    }
    if (nextStart >= s.size()) {
        return false;
    }
    attribute = s.substr(nextStart, s.size() - nextStart);
    return isSynonym(name) && isAttribute(attribute);
}

bool isAttribute(string s) {
    return regex_match(trim(s), regex("^(procName|varName|value|stmt#)$"));
}

bool isRef(string s)
{
    return isFixedString(s) || isInteger(s) || isAttrRef(s);
}
