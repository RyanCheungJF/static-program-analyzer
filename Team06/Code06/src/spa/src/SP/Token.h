#pragma once

#include <string>
#include "TokenType.h"

class Token {
public:
	Token();

	Token(TokenType type, std::string value);

	TokenType type;
	std::string value;

};