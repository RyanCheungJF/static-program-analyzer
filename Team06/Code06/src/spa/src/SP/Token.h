#pragma once

#include <string>
#include "TokenType.h"

class Token {
public:
	TokenType type;
	std::string value;

	Token(TokenType type, std::string value);
	std::string toString();
	friend bool operator==(const Token& lhs, const Token& rhs);
};