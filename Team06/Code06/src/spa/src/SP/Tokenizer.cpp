#include "Tokenizer.h"

std::deque<Token> Tokenizer::tokenize(std::stringstream& file) {
	std::deque<Token> tokens;

	while (file.peek() != EOF) {
		while (std::isspace(file.peek())) {
			file.get(); // Eat whitespace
		}

		if (std::isdigit(file.peek())) { // DIGIT: 0-9
			std::string tokenValue;
			tokenValue += (char)(file.get());
			/* Between two alphanumeric tokens, there must be at least one whitespace
			   in between them, so something like 123ab should throw an error. So we simply
			   fetch all the alphanumeric characters & rely on isInteger() to check if it's 
			   a valid integer.*/
			while (std::isalnum(file.peek())) {
				tokenValue += (char)(file.get());
			}
			if (!isInteger(tokenValue)) {
				throw SyntaxErrorException("Integer does not follow format: 0 | NZDIGIT ( DIGIT )*  -> " + tokenValue);
			}
			tokens.push_back(Token(TokenType::INTEGER, tokenValue));
		}
		else if (std::isalpha(file.peek())) { // LETTER: A-Z | a-z
			std::string tokenValue;
			tokenValue += (char)(file.get());
			while (std::isalnum(file.peek())) {
				tokenValue += (char)(file.get());
			}
			// Don't need a check here, since format for NAME -> LETTER (LETTER | DIGIT)*
			tokens.push_back(Token(TokenType::NAME, tokenValue));
		}
		else if ((char)file.peek() == '<') { // Handle <, <=
			file.get();
			if ((char)file.peek() == '=') {
				file.get();
				tokens.push_back(Token(TokenType::LESS_EQUAL, "<="));
			}
			else {
				tokens.push_back(Token(TokenType::LESS, "<"));
			}
		}
		else if ((char)file.peek() == '>') { // Handle >, >=
			file.get();
			if ((char)file.peek() == '=') {
				file.get();
				tokens.push_back(Token(TokenType::GREATER_EQUAL, ">="));
			}
			else {
				tokens.push_back(Token(TokenType::GREATER, ">"));
			}
		}
		else if ((char)file.peek() == '=') { // Handle =, ==
			file.get();
			if ((char)file.peek() == '=') {
				file.get();
				tokens.push_back(Token(TokenType::EQUAL, "=="));
			}
			else {
				tokens.push_back(Token(TokenType::ASSIGN, "="));
			}
		}
		else if ((char)file.peek() == '!') { // Handle !, !=
			file.get();
			if ((char)file.peek() == '=') {
				file.get();
				tokens.push_back(Token(TokenType::NOT_EQUAL, "!="));
			}
			else {
				tokens.push_back(Token(TokenType::NOT, "!"));
			}
		}
		else if ((char)file.peek() == '&') { // Handle &&
			std::string tokenValue;
			tokenValue += (char)(file.get());
			if ((char)file.peek() == '&') {
				file.get();
				tokens.push_back(Token(TokenType::AND, "&&"));
			}
			else {
				tokenValue += (char)(file.get());
				throw SyntaxErrorException("Expected &&, but got -> " + tokenValue);
			}
		}
		else if ((char)file.peek() == '|') { // Handle ||
			std::string tokenValue;
			tokenValue += (char)(file.get());
			if ((char)file.peek() == '|') {
				file.get();
				tokens.push_back(Token(TokenType::OR, "||"));
			}
			else {
				tokenValue += (char)(file.get());
				throw SyntaxErrorException("Expected ||, but got -> " + tokenValue);
			}
		}
		else { // Handle the rest of the tokens
			char tokenValue = (char)file.get();

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
				tokens.push_back(Token(TokenType::PLUS, "+"));
				break;
			case '-':
				tokens.push_back(Token(TokenType::MINUS, "-"));
				break;
			case '*':
				tokens.push_back(Token(TokenType::MULTIPLY, "*"));
				break;
			case '/':
				tokens.push_back(Token(TokenType::DIVIDE, "/"));
				break;
			case '%':
				tokens.push_back(Token(TokenType::MODULO, "%"));
				break;
			default:
				throw SyntaxErrorException("Invalid token");
			}
		}
	}
	tokens.push_back(Token(TokenType::ENDOFFILE, "End of File"));

	return tokens;
}

bool Tokenizer::isInteger(std::string value) {
	// INTEGER: 0 | NZDIGIT ( DIGIT )*
	std::regex integerRegex("^[1-9][0-9]*$");
	return value == "0" || std::regex_match(value, integerRegex);
}