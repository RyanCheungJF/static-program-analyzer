#include "Token.h"

Token::Token(TokenType type, std::string value) : type(std::move(type)), value(std::move(value)) {}

Token::Token(TokenType type) : type(std::move(type)), value() {}

bool Token::isType(TokenType tt) const {
    return type == tt;
}

bool Token::hasValue(std::string val) const {
    return value == val;
}

bool operator==(const Token& lhs, const Token& rhs) {
    return lhs.type == rhs.type && lhs.value == rhs.value;
}