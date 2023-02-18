//
// Created by Faruq on 26/1/23.
//

#include "QPSGrammarUtils.h"
#include <iostream>
#include <algorithm>
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
    return regex_match(integer, regex("^0$|^[1-9][0-9]*$"));
}

//bool isSelectClause(string selectClause) {
//    return regex_search(selectClause, regex("^Select"));
//}

//bool isSuchThatClause(string suchThatClause) {
//    return regex_match(suchThatClause, regex("^such that"));
//}

bool isSelect(string s) {
    return regex_search(s, regex("^Select$"));
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
    return regex_search(designEntity, regex("^(stmt|read|print|call|while|if|assign|variable|constant|procedure)"));
}

//TODO: Does this belong in parser util?
pair<string,string> extractDesignEntity(string designEntity) {
    regex rgx("^(stmt|read|print|call|while|if|assign|variable|constant|procedure)\\s+");
    smatch match;
    string remainder;
    if (regex_search(designEntity, match, rgx)) {
        remainder = match.suffix().str();
    }
    return pair(match[1], remainder);
}

bool isFixedString(string s) {
    return regex_match(s, regex("^\"[a-zA-Z][a-zA-Z0-9]*\"$"));
}

bool isWildCard(string s) {
    return s == "_";
}

bool isStmtRef(string stmtRef) {
    return isSynonym(stmtRef) || isInteger(stmtRef) || isWildCard(stmtRef);
}

bool isEntRef(string entRef) {
    return isSynonym(entRef) || isWildCard(entRef) || isFixedString(entRef);
}



//TODO: implement this, just do string matching, i.e. check if given string is the correct keyword
//bool QPSGrammarUtils::isFollows(string)
//bool QPSGrammarUtils::isParent(string)
//bool QPSGrammarUtils::isUses(string)
//bool QPSGrammarUtils::isModifies(string)

//TODO: Implement this
bool isExprSpec(string s)
{
    s = trim(s);
    if (s.empty()) {
        return false;
    }
    if (s == "_") {
        return true;
    }
    // removes all whitespace from s.
    s = removeCharFromString(s, ' ');
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
    if(startsWithQuotation && endsWithQuotation) {
        if (s.size() < 3) {
            return false;
        }
        // This will get rid of _" and "_
        // If s = _"X+Y"_ then expr = X+Y
        string expr = s.substr(1, s.size() - 2);
        // this aaa is for debugging purposes
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
    // finds the first + or - from the back where there are no brackets encapsulating them.
    for(int i = s.size() - 1; i >= 0; i--) {
        if ((s[i] == '+' || s[i] == '-') && bracketsCounter == 0) {
            index = i;
            break;
        }
        if (s[i] == '(') {
            bracketsCounter--;
        } else if (s[i] == ')') {
            bracketsCounter++;
        }
    }
//    int index = s.size() - 1 - m.position();
    if (index == -1) {
        // regex search not found
        return isTerm(s);
    }
    if (index == s.size()-1 || index == 0) {
        // cannot have + or - at start or end of string
        return false;
    }
    string first = trim(s.substr(0, index));
    string second = trim(s.substr(index+1, s.size()-1-index));
    // this aaa bbb is for debugging purposes
    return isExpr(first) && isTerm(second);
}

bool isTerm(string s) {
    if (s.empty()) {
        return false;
    }
    // finds the first */% from the back where there are no brackets encapsulating them.
    int index = -1;
    int bracketsCounter = 0;
    for(int i = s.size() - 1; i >= 0; i--) {
        if ((s[i] == '*' || s[i] == '/' || s[i] == '%') && bracketsCounter == 0) {
            index = i;
            break;
        }
        if (s[i] == '(') {
            bracketsCounter--;
        } else if (s[i] == ')') {
            bracketsCounter++;
        }
    }
    if (index == -1) {
        // None match found
        return isFactor(s);
    }
    if (index == s.size()-1 || index == 0) {
        // operator cannot be at start and end of string
        return false;
    }
    string first = trim(s.substr(0, index));
    string second = trim(s.substr(index+1, s.size()-1-index));
    return isTerm(first) && isFactor(second);
}

bool isFactor(string s) {
    if (s.empty()) {
        return false;
    }
    // recursively remove brackets
    bool hasBrackets = false;
    if (s[0] == '(' && s[s.size()-1] == ')') {
        hasBrackets = true;
        s = s.substr(1, s.size()-2);
    }
    if (hasBrackets){
        return isExpr(s);
    }
    return isName(s) || isInteger(s);
}

bool isFollows(string s) {
    return s == "Follows";
}

bool isParent(string s) {
    return s == "Parent";
}

bool isUses(string s) {
    return s == "Uses";
}

bool isModifies(string s) {
    return s == "Modifies";
}

//bool isPatternCl(string s)
//{
//    return s == "pattern";
//}


bool isRelRef(string s) {
    return isFollows(s) || isParent(s) || isUses(s) || isModifies(s);
}