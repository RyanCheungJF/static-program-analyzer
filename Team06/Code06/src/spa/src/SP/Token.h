#pragma once

#include "TokenType.h"
#include <string>

class Token {
public:
  TokenType type;
  std::string value;

  Token(TokenType type, std::string value);
  Token(TokenType type);

  bool isType(TokenType tt);
  bool hasValue(std::string val);
  friend bool operator==(const Token &lhs, const Token &rhs);
};