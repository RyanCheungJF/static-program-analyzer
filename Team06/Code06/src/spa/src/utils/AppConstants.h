#pragma once

#include <stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef std::string ProcName;
typedef std::string Stmt;
typedef std::string Ent;
typedef std::string Operator;
typedef int StmtNum;
typedef std::string Const;
typedef std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> CFG;

class AppConstants {
public:
    inline static const std::string READ = "read";
    inline static const std::string ASSIGN = "assign";
    inline static const std::string PRINT = "print";
    inline static const std::string CALL = "call";
    inline static const std::string IF = "if";
    inline static const std::string WHILE = "while";
    inline static const std::string STMT = "stmt";
    inline static const std::string VARIABLE = "variable";
    inline static const std::string CONSTANT = "constant";
    inline static const std::string PROCEDURE = "procedure";
    inline static const std::string SYNONYM = "synonym";
    inline static const std::string WILDCARD = "wildcard";
    inline static const std::string FIXED_INT = "fixed_int";
    inline static const std::string BOOLEAN = "BOOLEAN";
    inline static const std::string FIXED_STRING = "fixed_string";

    inline static const std::string WILDCARD_VALUE = "_";

    // synonym attributes
    inline static const std::string PROCNAME = "procName";
    inline static const std::string VARNAME = "varName";
    inline static const std::string VALUE = "value";
    inline static const std::string STMTNO = "stmt#";

    // relationships
    inline static const std::string FOLLOWS = "Follows";
    inline static const std::string FOLLOWST = "Follows*";
    inline static const std::string PARENT = "Parent";
    inline static const std::string PARENTT = "Parent*";
    inline static const std::string USES = "Uses";
    inline static const std::string MODIFIES = "Modifies";
    inline static const std::string NEXT = "Next";
    inline static const std::string NEXTT = "Next*";
    inline static const std::string CALLS = "Calls";
    inline static const std::string CALLST = "Calls*";
    inline static const std::string AFFECTS = "Affects";
    inline static const std::string AFFECTST = "Affects*";

    // comparisons
    inline static const std::string OP_EQUALS = "=";

    // terminals
    inline static const std::string THEN = "then";
    inline static const std::string ELSE = "else";
    inline static const std::string AND = "&&";
    inline static const std::string OR = "||";
    inline static const std::string EQUALS = "==";
    inline static const std::string NOT_EQUALS = "!=";
    inline static const char NOT = '!';
    inline static const char GREATER = '>';
    inline static const char LESS = '<';
    inline static const char PLUS = '+';
    inline static const char MINUS = '-';
    inline static const char MULTIPLY = '*';
    inline static const char DIVIDE = '/';
    inline static const char MODULO = '%';
    inline static const char LEFT_BRACE = '{';
    inline static const char RIGHT_BRACE = '}';
    inline static const char LEFT_PARENTHESIS = '(';
    inline static const char RIGHT_PARENTHESIS = ')';
    inline static const char SEMICOLON = ';';
    inline static const char EQUAL_SIGN = '=';
    inline static const char AMPERSAND = '&';
    inline static const char VERTICAL_BAR = '|';

    inline static const std::string STRING_NOT = "!";
    inline static const std::string STRING_GREATER = ">";
    inline static const std::string STRING_LESS = "<";
    inline static const std::string STRING_PLUS = "+";
    inline static const std::string STRING_MINUS = "-";
    inline static const std::string STRING_MULTIPLY = "*";
    inline static const std::string STRING_DIVIDE = "/";
    inline static const std::string STRING_MODULO = "%";
    inline static const std::string STRING_LEFT_PARENTHESIS = "(";
    inline static const std::string STRING_RIGHT_PARENTHESIS = ")";
    inline static const std::string STRING_SEMICOLON = ";";
    inline static const std::string STRING_EQUAL_SIGN = "=";
    inline static const std::string STRING_AMPERSAND = "&";
    inline static const std::string STRING_VERTICAL_BAR = "|";

    // CFG-related
    inline static const std::string PARENTS = "parents";
    inline static const std::string CHILDREN = "children";

    inline static const int NOT_USED_FIELD = -1;
    inline static const int DUMMY_NODE = 0;
    inline static const std::string PROCEDURE_DOES_NOT_EXIST = "procedure does not exist";
    inline static const int IS_FIND_CHILDREN = true;

    // EvalPriority related
    inline static const double wildcardWeight = 100;
    inline static const double wildcardWeightEarlyReturn = 9999;
    inline static const double stmtWeight = 100;
    inline static const double fixedValWeight = 10000;
    inline static const double otherWeight = 1000;
    inline static const double typeWeight = 1;
    inline static const double highestPriority = 100000;
};