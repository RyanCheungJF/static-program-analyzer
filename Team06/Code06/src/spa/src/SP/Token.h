#pragma once

#include <string>
#include "TokenType.h"

class Token {
public:
	TokenType type;
	std::string value;

	Token();

	Token(TokenType type, std::string value);

	std::string toString();
};