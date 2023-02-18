#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

typedef std::string ProcName;
typedef std::string Stmt;
typedef std::string Ent;
typedef int StmtNum;
typedef int Const;

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
    inline static const std::string FIXED_STRING = "fixed_string";
    inline static const std::string FIXED_STRING_WTIH_WILDCARD = "fixed_string_with_wildcard";

    // relationships
    inline static const std::string FOLLOWS = "Follows";
    inline static const std::string FOLLOWST = "Follows*";
    inline static const std::string PARENT = "Parent";
    inline static const std::string PARENTT = "Parent*";
    inline static const std::string USES = "Uses";
    inline static const std::string MODIFIES = "Modifies";
};