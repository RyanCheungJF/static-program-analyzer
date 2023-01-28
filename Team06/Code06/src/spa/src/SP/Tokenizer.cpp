#include "Tokenizer.h"

Tokenizer::Tokenizer() {

}

std::deque<Token> Tokenizer::tokenize(std::stringstream& file) {
	std::deque<Token> tokens;

	while (file.peek() != EOF) {
		// Eat whitespace
		while (((char)file.peek() == '\n' || (char)file.peek() == '\r' || (char)file.peek() == '\t' || (char)file.peek() == ' ' || (char)file.peek() == '\f' || (char)file.peek() == '\v')) {
			file.get();
		}

		if ('0' <= (char)file.peek() && (char)file.peek() <= '9') { // Number
			std::string tokenValue;
			tokenValue += (char)(file.get());
			while ('0' <= (char)file.peek() && (char)file.peek() <= '9') {
				tokenValue += (char)(file.get());
			}
			if (tokenValue.length() > 1 && tokenValue[0] == '0') { // INTEGER: 0 | NZDIGIT ( DIGIT )*
				std::cerr << "Integer with more than 1 digit cannot start with 0, " << tokenValue << std::endl;
			}
			tokens.push_back(Token(TokenType::INTEGER, tokenValue));
		} else if (('a' <= (char)file.peek() && (char)file.peek() <= 'z' || 'A' <= (char)file.peek() && (char)file.peek() <= 'Z')) { // Name (Could be keyword as well)
			std::string tokenValue;
			tokenValue += (char)(file.get());
			while ('a' <= (char)file.peek() && (char)file.peek() <= 'z' || 'A' <= (char)file.peek() && (char)file.peek() <= 'Z') {
				tokenValue += (char)(file.get());
			}
			tokens.push_back(Token(TokenType::NAME, tokenValue));
		}
		else if ((char)file.peek() == '<') {
			std::string tokenValue;
			tokenValue += (char)(file.get());
			if ((char)file.peek() == '=') {
				tokenValue += (char)(file.get());
				tokens.push_back(Token(TokenType::LESS_EQUAL, tokenValue));
			}
			else {
				tokens.push_back(Token(TokenType::LESS, tokenValue));
			}
		} else if ((char)file.peek() == '>') {
			std::string tokenValue;
			tokenValue += (char)(file.get());
			if ((char)file.peek() == '=') {
				tokenValue += (char)(file.get());
				tokens.push_back(Token(TokenType::GREATER_EQUAL, tokenValue));
			}
			else {
				tokens.push_back(Token(TokenType::GREATER, tokenValue));
			}
		} else if ((char)file.peek() == '=') {
			std::string tokenValue;
			tokenValue += (char)(file.get());
			if ((char)file.peek() == '=') {
				tokenValue += (char)(file.get());
				tokens.push_back(Token(TokenType::EQUAL, tokenValue));
			}
			else {
				tokens.push_back(Token(TokenType::ASSIGN, tokenValue));
			}
		} else if ((char)file.peek() == '!') {
			std::string tokenValue;
			tokenValue += (char)(file.get());
			if ((char)file.peek() == '=') {
				tokenValue += (char)(file.get());
				tokens.push_back(Token(TokenType::NOT_EQUAL, tokenValue));
			}
			else {
				tokens.push_back(Token(TokenType::NOT, tokenValue));
			}
		} else if ((char)file.peek() == '&') {
			std::string tokenValue;
			tokenValue += (char)(file.get());
			if ((char)file.peek() == '&') {
				tokenValue += (char)(file.get());
				tokens.push_back(Token(TokenType::AND, tokenValue));
			}
			else {
				tokenValue += (char)(file.get());
				std::cerr << "Expecting &&, but got: " << tokenValue << std::endl;
			}
		} else if ((char)file.peek() == '|') {
			std::string tokenValue;
			tokenValue += (char)(file.get());
			if ((char)file.peek() == '|') {
				tokenValue += (char)(file.get());
				tokens.push_back(Token(TokenType::OR, tokenValue));
			} else {
				tokenValue += (char)(file.get());
				std::cerr << "Expecting ||, but got: " << tokenValue << std::endl;
			}
		} else {
			char tokenValue = (char)file.get();
			Token token;

			switch (tokenValue) {
				case '{': 
					tokens.push_back(Token(TokenType::LEFT_BRACE, "{"));
					break;
				case '}':
					tokens.push_back(Token(TokenType::RIGHT_BRACE, "}"));
					break;
				case '(':
					tokens.push_back(Token(TokenType::LEFT_PARENTHESIS, "("));
					break;
				case ')':
					tokens.push_back(Token(TokenType::RIGHT_PARENTHESIS, ")"));
					break;
				case ';':
					tokens.push_back(Token(TokenType::SEMICOLON, ";"));
					break;
				case '+':
					tokens.push_back(Token(TokenType::LEFT_BRACE, "{"));
					break;
				case '-':
					tokens.push_back(Token(TokenType::LEFT_BRACE, "{"));
					break;
				case '*':
					tokens.push_back(Token(TokenType::LEFT_BRACE, "{"));
					break;
				case '/':
					tokens.push_back(Token(TokenType::LEFT_BRACE, "{"));
					break;
				case '%':
					tokens.push_back(Token(TokenType::LEFT_BRACE, "{"));
					break;
				case '\0':
					break;
				default:
					std::cerr << "Invalid token: " << tokenValue << std::endl;
			}
		}
	}
	tokens.push_back(Token(TokenType::ENDOFFILE, "EOF"));

	for (int i = 0; i < tokens.size(); i++) {
		std::cout << tokens[i].toString() << std::endl;
	}

	return tokens;

}