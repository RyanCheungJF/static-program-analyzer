#include "Token.h"

Token::Token(TokenType type, std::string value) {
	this->type = type;
	this->value = value;
}

bool operator==(const Token& lhs, const Token& rhs) {
	return lhs.type == rhs.type && lhs.value == rhs.value;
}