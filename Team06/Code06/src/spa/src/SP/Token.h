#pragma once

#include <string>

#include "TokenType.h"

class Token {
public:
    TokenType type;
    std::string value;

    Token(TokenType type, std::string value);
    explicit Token(TokenType type);

    bool isType(const TokenType& tt) const;
    bool hasValue(const std::string& val) const;
    friend bool operator==(const Token& lhs, const Token& rhs);
};